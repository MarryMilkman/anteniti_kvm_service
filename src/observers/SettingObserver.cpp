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
		this->_execute_list_request();
		this->_check_untreated_list_request();
	}
}

void 	SettingObserver::_execute_list_request() {
	this->_refresh_untreated_list_request(eRequestType::rt_SettingRequest);

	for (Request &request : this->_list_untreated_request) {
		if (request.task_ptr)
			continue;
		try {
			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
			while (1) {
				try {
					std::string 	title = mesh.list_serial_number[0] + " setting change";
					std::string 	message = SETTING_CHENGED;

					message += "\n***DELIM***\n";
					request.task_ptr = this->_task_controller.make_new_task(title, mesh.tcp_ip, message);
					break;
				}
				catch (std::exception &ref){
					continue;
				}
			}
		} catch (std::exception &e) {
			continue;
		}
	}
}
