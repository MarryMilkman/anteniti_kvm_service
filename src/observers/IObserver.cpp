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

	for (std::shared_ptr<MySQLDataSegment>	mysql_data : list_mysql_data) {
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

		request.number_check++;
		if (!request.task_ptr) {
			i++;
			continue;
		}
		if (request.task_ptr->status >= eTaskStatus::ts_Finish) {
			request.mysql_data->answer_message = request.task_ptr->answer_message;
			request.mysql_data->status = eRequestStatus::rs_Finish;
			request.task_ptr->status = eTaskStatus::ts_Used;
			this->_mysql_controller.story(request.mysql_data);
			request.mysql_data = 0;
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
