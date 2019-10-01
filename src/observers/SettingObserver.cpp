/*
	SettingObserver
class to track about new setting-request
*/




#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/SettingObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

SettingObserver::SettingObserver() :
	_loger("../logs/setting_observer.logs")
{
}

SettingObserver::~SettingObserver() {
}

SettingObserver	&SettingObserver::getInstance() {
	static SettingObserver info_obs;

	return info_obs;
}

void 	SettingObserver::operator()() {
	while (1) {
		this->_execute_list_request(SETTING_CHANGED, eRequestType::rt_SettingRequest);
		this->_check_untreated_list_request();
	}
}

// MARK : - _execute_list_request
//	1) check new request in mysql (with help of _mysql_controller) and story tham to _list_untreated_request
//	2) from request get imei and name_mesh for founding correct Mesh-network
//		if find -> make new Task (_task_controller.make_task), and story tham in current request
// 		if not find -> current request stay without Task (CAN_NOT_FIND_MESH)
// void 	SettingObserver::_execute_list_request() {
// 	if (!this->_list_untreated_request.size())
// 		this->_refresh_untreated_list_request(eRequestType::rt_SettingRequest);
// 	for (Request &request : this->_list_untreated_request) {
// 		if (request.task_ptr)
// 			continue;
// 		try {
// 			request.is_mesh_find = false;
// 			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
//
// 			request.is_mesh_find = true;
//
// 			std::string 	title = mesh.list_serial_number[0] + " setting change";
// 			std::string 	message;
//
// 			message = std::string("Command") + std::string("\n***DELIM***\n");
// 			message += SETTING_CHANGED;
// 			// while (!request.task_ptr)
// 				request.task_ptr = this->_task_controller.make_new_task(title, mesh.tcp_ip, message, 6);
// 		} catch (std::exception &e) {
// 			continue;
// 		}
// 	}
// }
