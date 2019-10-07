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

eLockStatus 		PortController::try_reserv_port(int port, eLockStatus type_lock) {
	if (!this->_map_ports.count(port)) {
		std::lock_guard<std::mutex>	gu_lock(this->_mutex);

		if (!this->_map_ports.count(port))
			this->_map_ports[port] = PortController::Port(port);
	}
	eLockStatus	status = this->_map_ports[port].lock_status;

	if (type_lock == eLockStatus::ls_LockForUse)
		return this->_persistent_reserv(port, type_lock);

	if (status == eLockStatus::ls_InUse || status == eLockStatus::ls_InCheck)
		return status;
/////////////////////////////
	{
		std::lock_guard<std::mutex>	gu_lock(this->_map_ports[port].p_mutex);

		status = this->_map_ports[port].lock_status;
		if (status == eLockStatus::ls_InUse || status == eLockStatus::ls_InCheck)
			return status;

		this->_map_ports[port].lock_status = type_lock;
	}
	return eLockStatus::ls_ReservSeccess;
}

void 		PortController::unreserv_port(int port) {
	if (!this->_map_ports.count(port)) {
		std::lock_guard<std::mutex>	gu_lock(this->_mutex);

		if (!this->_map_ports.count(port))
			this->_map_ports[port] = PortController::Port(port);
	}
	std::lock_guard<std::mutex>	gu_lock(this->_map_ports[port].p_mutex);

	this->_map_ports[port].lock_status = eLockStatus::ls_Free;
}


eLockStatus 		PortController::_persistent_reserv(int port, eLockStatus type_lock) {
	std::lock_guard<std::mutex>	gu_lock(this->_map_ports[port].p_mutex);

	if (this->_map_ports[port].lock_status == ls_InUse)
		return eLockStatus::ls_InUse;
	this->_map_ports[port].lock_status = type_lock;
	return eLockStatus::ls_ReservSeccess;
}


/////////////// class PortController::Port

PortController::Port::Port() {
	this->value = 0;
	this->lock_status = eLockStatus::ls_Free;
}

PortController::Port::Port(int port) {
	this->value = port;
	this->lock_status = eLockStatus::ls_Free;
}


PortController::Port::~Port() {
}

PortController::Port::Port(Port const &ref) {
	*this = ref;
}

PortController::Port 			&PortController::Port::operator=(Port const &ref) {
	this->value = ref.value;
	this->lock_status = ref.lock_status;
	return *this;
}
