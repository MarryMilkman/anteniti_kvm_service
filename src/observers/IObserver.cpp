#include "observers/IObserver.hpp"
#include "controllers/MySQLController.hpp"
#include "controllers/TaskController.hpp"
#include "controllers/MeshController.hpp"
#include "Request.hpp"
#include "MySQLDataSegment.hpp"
#include "Mesh.hpp"

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
	std::vector<MySQLDataSegment *> 	list_mysql_data = this->_mysql_controller.get_request(type_request);

	for (MySQLDataSegment *mysql_data : list_mysql_data) {
		if (mysql_data->status == eRequestStatus::rs_NotProcessed) {
			mysql_data->status = eRequestStatus::rs_InProgress;
			this->_list_untreated_request.push_back(Request(mysql_data));
		}
	}
}

//	check every request in _list_untreated_request.
//		if task finish => check tcp_ip (is send/apply executed correct ?
// 				story to mysql answer and delete request : refresh connection to mesh and try repeat tesk)
void 	IObserver::_check_untreated_list_request() {
	for (unsigned int i = 0; i < this->_list_untreated_request.size();) {
		Request 	&request = this->_list_untreated_request[i];
		Task 		*task = request.task_ptr.get();

		request.number_check++;
		if (task->status == eTaskStatus::ts_Finish) {
			if (task->answer_message == TASK_FAIL_BROKEN_TCP_IP) {
				try {
					Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
					this->_mesh_controller.refresh_connection(mesh);
					request.task_ptr = this->_task_controller.make_new_task(task->title, mesh.tcp_ip, task->message);
				} catch (std::exception &ref) {
					continue ;
				}
				i++;
				continue;
			}
			request.mysql_data->status = eRequestStatus::rs_Finish;
			this->_mysql_controller.story(request.mysql_data);
			this->_list_untreated_request.erase(this->_list_untreated_request.begin() + i);
		}
		// if (request.number_check > 10) {
		//
		// }
		i++;
	}
}
