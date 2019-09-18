#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/BlockingObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

BlockingObserver::BlockingObserver(){
}

BlockingObserver::~BlockingObserver() {
}

BlockingObserver	&BlockingObserver::getInstance() {
	static BlockingObserver info_obs;

	return info_obs;
}

void 	BlockingObserver::operator()() {
	while (1) {
		this->_execute_list_request();
		this->_check_untreated_list_request();
	}
}

void 	BlockingObserver::_execute_list_request() {
	this->_refresh_untreated_list_request(eRequestType::rd_InfoRequest);

	for (Request request : this->_list_untreated_request) {
		if (request.task_ptr)
			continue;
		try {
			Mesh &mesh = this->_mesh_controller.get_mesh_by(request.mysql_data->imei, request.mysql_data->name_mesh);
			while (1) {
				try {
					std::string 	title = mesh.list_serial_number[0] + " block devices";
					std::string 	message = BLOCK_DEVICE;

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
