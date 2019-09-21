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
		this->_execute_list_request();
		this->_check_untreated_list_request();
	}
}

void 	InfoObserver::_execute_list_request() {
	this->_refresh_untreated_list_request(eRequestType::rt_InfoRequest);

	for (Request &request : this->_list_untreated_request) {
		if (request.task_ptr)
			continue;
		try {
			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
			while (1) {
				try {
					std::string 	title = mesh.list_serial_number[0] + " get info";
					std::string 	message = SEND_INFO;

					message += std::string("\n***DELIM***\n") + std::string("...");
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
