/*
	InfoObserver
class to track about new info-request
*/




#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/InfoObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

InfoObserver::InfoObserver() :
	_loger("../logs/info_observer.logs")
{
}

InfoObserver::~InfoObserver() {
}

InfoObserver	&InfoObserver::getInstance() {
	static InfoObserver info_obs;

	return info_obs;
}

void 	InfoObserver::operator()() {
	while (1) {
		// std::cerr << "jojo\n";
		this->_execute_list_request(SEND_INFO, eRequestType::rt_InfoRequest);
		this->_check_untreated_list_request();
	}
}

// MARK : - _execute_list_request
//	1) check new request in mysql (with help of _mysql_controller) and story tham to _list_untreated_request
//	2) from request get imei and name_mesh for founding correct Mesh-network
//		if find -> make new Task (_task_controller.make_task), and story tham in current request
// 		if not find -> current request stay without Task (CAN_NOT_FIND_MESH)
// void 	InfoObserver::_execute_list_request() {
// 	if (!this->_list_untreated_request.size())
// 		this->_refresh_untreated_list_request(eRequestType::rt_InfoRequest);
// 	for (Request &request : this->_list_untreated_request) {
// 		if (request.task_ptr)
// 			continue;
// 		try {
// 			request.is_mesh_find = false;
// 			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
//
// 			request.is_mesh_find = true;
//
// 			std::string 	title = mesh.list_serial_number[0] + " get info";
// 			std::string 	message;
//
// 			message = std::string("Command") + std::string("\n***DELIM***\n");
// 			message += SEND_INFO;
// 			// while (!request.task_ptr)
// 				request.task_ptr = this->_task_controller.make_new_task(title, mesh.tcp_ip, message, 6);
// 		} catch (std::exception &e) {
// 			continue;
// 		}
// 	}
// }
