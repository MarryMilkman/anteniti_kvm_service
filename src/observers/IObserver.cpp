/*
	IObserver
Interface of observers.
virtual metods: _refresh_untreated_list_request, _check_untreated_list_request
for uses need override    operator();
*/


#include "observers/IObserver.hpp"
#include "controllers/MySQLController.hpp"
#include "controllers/TaskController.hpp"
#include "controllers/MeshController.hpp"
#include "Request.hpp"
#include "MySQLDataSegment.hpp"
#include "Mesh.hpp"
#include "Loger.hpp"
#include "Parser.hpp"
#include "Tunnel.hpp"

IObserver::IObserver() :
	_mysql_controller(MySQLController::getInstance()),
	_task_controller(TaskController::getInstance()),
	_mesh_controller(MeshController::getInstance())
{
}

IObserver::~IObserver() {

}

// MARK : - _execute_list_request
//	1) check new request in mysql (with help of _mysql_controller) and story tham to _list_untreated_request
//	2) from request get imei and name_mesh for founding correct Mesh-network
//		if find -> make new Task (_task_controller.make_task), and story tham in current request
// 		if not find -> current request stay without Task (CAN_NOT_FIND_MESH)
void 	IObserver::_execute_list_request(std::string message_for_task, eRequestType type_request, int timeout) {
	if (!this->_list_untreated_request.size())
		this->_refresh_untreated_list_request(type_request);
	for (Request &request : this->_list_untreated_request) {
		if (request.task_ptr)
			continue;
		try {
			request.is_mesh_find = false;
			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
			if (!mesh.tunnel || !mesh.tunnel->is_available)
				mesh.refresh_connection();

			request.is_mesh_find = true;

			std::string 	title = mesh.list_serial_number[0] + " " + message_for_task;
			std::string 	message;

			message = std::string("Command") + std::string("-") + mesh.list_serial_number[0] + std::string("\n***DELIM***\n");
			message += message_for_task;
			// while (!request.task_ptr)
			request.task_ptr = this->_task_controller.make_new_task(title, mesh.tunnel, message, timeout);
		} catch (std::exception &e) {
			continue;
		}
	}
}






// get data from mysql and story untreated data to _list_untreated_request + change status data
void 		IObserver::_refresh_untreated_list_request(eRequestType type_request) {
	std::vector<std::shared_ptr<MySQLDataSegment>> 	list_mysql_data = this->_mysql_controller.get_request(type_request);

	for (std::shared_ptr<MySQLDataSegment>	&mysql_data : list_mysql_data) {
		if (mysql_data->status == eRequestStatus::rs_NotProcessed) {
			mysql_data->status = eRequestStatus::rs_InProgress;
			this->_list_untreated_request.push_back(Request(mysql_data));
		}
	}
}

//	check every request in _list_untreated_request.
//		if no task_ptr and no true in flags is_mesh_find
//			story to mysql request CAN_NOT_FIND_MESH
//		if task_ptr finish => check tunnel (is send/apply executed correct ?
// 				story to mysql answer and delete request : refresh connection to mesh and try repeat tesk (2 retray))
//			if answer == INCORRECT_ADDRESSEE
//					its mean, that current port not valid, and need refresh connection for both mesh
void 	IObserver::_check_untreated_list_request() {
	unsigned int 		size = this->_list_untreated_request.size();

	for (unsigned int i = 0; i < size;) {
		Request 				&request = this->_list_untreated_request[i];

		if (!request.task_ptr) {
			if (request.is_mesh_find) {
				i++;
				continue;
			}
			std::cerr << "------------ATANTION! no task for request (CAN_NOT_FIND_MESH)-----------------\n";
			request.mysql_data->answer_message = CAN_NOT_FIND_MESH;
			request.mysql_data->status = eRequestStatus::rs_Finish;
			this->_mysql_controller.story(request.mysql_data);
			// request.task_ptr = this->_task_controller.make_new_task(request.itle, mesh.tunnel, request.message);
			this->_list_untreated_request.erase(this->_list_untreated_request.begin() + i);
			size = this->_list_untreated_request.size();
			continue;
		}

		if (request.task_ptr->status >= eTaskStatus::ts_Finish) {
			std::vector<std::string>	answer_segment = Parser::custom_split(request.task_ptr->answer_message, " ");
			std::string 				answer = answer_segment[0];
			std::shared_ptr<Task> 		&task_ptr = request.task_ptr;

			request.number_check++;
			if (answer == INCORRECT_ADDRESSEE) {
				std::cerr << "---------INCORRECT_ADDRESSEE--------try again-----------\n";
				try {

					Mesh 	&incorrect_mesh = this->_mesh_controller.get_mesh_by(answer_segment[1]);
					{
						std::cerr << "ATENTION! cen be segmentation prikol\n";
						std::lock_guard<std::mutex>		lock(incorrect_mesh.tunnel->s_mutex);
						incorrect_mesh.tunnel->is_available = false;
						incorrect_mesh.tunnel = 0;
					}
					Mesh 	&mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
					mesh.refresh_connection();

					std::string 	title = task_ptr->title;
					std::string 	message = task_ptr->message;
					int 			timeout = task_ptr->timeout;

					request.task_ptr->status = eTaskStatus::ts_Used;
					request.task_ptr = this->_task_controller.make_new_task(title, mesh.tunnel, message, timeout);
					i++;
					continue;
				} catch (std::exception &e) {}
			}

			if ( (answer == TASK_FAIL_BROKEN_TCP_IP && request.number_check < 2)) {
				std::cerr << "---TASK_FAIL_BROKEN_TCP_IP ----try apply request again----------------\n";

				try {
					Mesh 	&mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
					mesh.refresh_connection();
					std::string 	title = task_ptr->title;
					std::string 	message = task_ptr->message;
					int 			timeout = task_ptr->timeout;

					request.task_ptr->status = eTaskStatus::ts_Used;
					request.task_ptr = this->_task_controller.make_new_task(title, mesh.tunnel, message, timeout);
					i++;
					continue;
				} catch (std::exception &e) {}
			}
			request.mysql_data->answer_message = request.task_ptr->answer_message;
			request.mysql_data->status = eRequestStatus::rs_Finish;
			request.task_ptr->status = eTaskStatus::ts_Used;
			this->_mysql_controller.story(request.mysql_data);
			this->_list_untreated_request.erase(this->_list_untreated_request.begin() + i);
			size = this->_list_untreated_request.size();
			continue;
		}
		i++;
	}
}
