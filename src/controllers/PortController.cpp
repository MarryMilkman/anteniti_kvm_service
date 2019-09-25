#include "controllers/PortController.hpp"

PortController::PortController()
{
}

PortController::~PortController() {

}

PortController	&PortController::getInstance() {
	static PortController port_controller;

	return port_controller;
}

bool 		PortController::try_reserv_port(int port) {
	if (!this->_map_mutex.count(port)) {
		_map_mutex[port] = std::shared_ptr<std::mutex>(new std::mutex());
	}

	{
		std::lock_guard<std::mutex>	gu_lock(*_map_mutex[port]);

		if (this->_map_ports.count(port) <= 0 || this->_map_ports[port] == 0) {
			this->_map_ports[port] = 1;
			return true;
		}
	}
	return false;
}

void 		PortController::unreserv_port(int port) {
	if (!this->_map_mutex.count(port)) {
		_map_mutex[port] = std::shared_ptr<std::mutex>(new std::mutex());
	}
	std::lock_guard<std::mutex>	gu_lock(*_map_mutex[port]);

	this->_map_ports[port] = 0;
}
