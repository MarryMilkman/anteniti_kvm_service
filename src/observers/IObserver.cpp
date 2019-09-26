#include "observers/IObserver.hpp"
#include "controllers/MySQLController.hpp"
#include "controllers/TaskController.hpp"
#include "controllers/MeshController.hpp"
#include "Request.hpp"
#include "MySQLDataSegment.hpp"
#include "Mesh.hpp"
#include "Loger.hpp"

IObserver::IObserver() :
	_mysql_controller(MySQLController::getInstance()),
	_task_controller(TaskController::getInstance()),
	_mesh_controller(MeshController::getInstance())
{
}

IObserver::~IObserver() {

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
//		if task_ptr finish => check tcp_ip (is send/apply executed correct ?
// 				story to mysql answer and delete request : refresh connection to mesh and try repeat tesk)
void 	IObserver::_check_untreated_list_request() {
	unsigned int 		size = this->_list_untreated_request.size();

	for (unsigned int i = 0; i < size;) {
		Request 				&request = this->_list_untreated_request[i];

		if (!request.task_ptr) {
			std::cerr << "------------ATANTION! no task for request (CAN_NOT_FIND_MESH)-----------------\n";
			request.mysql_data->answer_message = CAN_NOT_FIND_MESH;
			request.mysql_data->status = eRequestStatus::rs_Finish;
			this->_mysql_controller.story(request.mysql_data);
			// request.task_ptr = this->_task_controller.make_new_task(request.itle, mesh.tcp_ip, request.message);
			this->_list_untreated_request.erase(this->_list_untreated_request.begin() + i);
			size = this->_list_untreated_request.size();
			continue;
		}
		if (request.task_ptr->status >= eTaskStatus::ts_Finish) {
			request.number_check++;
			// std::cerr << request.number_check << " ------ number check\n";
			if (request.task_ptr->answer_message == TASK_FAIL_BROKEN_TCP_IP && request.number_check < 2) {
				std::cerr << "---TASK_FAIL_BROKEN_TCP_IP----try apply request again----------------\n";

				std::shared_ptr<Task> task_ptr = request.task_ptr;
				try {
					Mesh 	&mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
					this->_mesh_controller.refresh_connection(mesh);
					std::string 	title = task_ptr->title;
					std::string 	message = task_ptr->message;

					request.task_ptr->status = eTaskStatus::ts_Used;
					request.task_ptr = this->_task_controller.make_new_task(title, mesh.tcp_ip, message);
					i++;
					continue;
				} catch (std::exception &e) {}
			}
			// std::stringstream ss_message;
			// ss_message << request.task_ptr->answer_message << " : " << request.task_ptr << "*" << request.task_ptr->status << "******************\n";
			// std::cerr << ss_message.str();
			request.mysql_data->answer_message = request.task_ptr->answer_message;
			request.mysql_data->status = eRequestStatus::rs_Finish;
			request.task_ptr->status = eTaskStatus::ts_Used;
			this->_mysql_controller.story(request.mysql_data);
			this->_list_untreated_request.erase(this->_list_untreated_request.begin() + i);
			size = this->_list_untreated_request.size();
			continue;
		}
		// if (request.number_check > 10) {
		//
		// }
		i++;
	}
}
