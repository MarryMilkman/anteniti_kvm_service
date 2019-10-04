#include "controllers/PortController.hpp"
#include "TCP_IP.hpp"

PortController::PortController()
{
}

PortController::~PortController() {

}

PortController	&PortController::getInstance() {
	static PortController port_controller;

	return port_controller;
}

void 				PortController::watch_used_port() {
	while (1) {
		for (auto &item : this->_map_ports) {
			Port &port = item.second;

			if (port.lock_status == ls_Free)
				continue;
			time_t 	now;

			time(&now);
			if ((now - port.tcp_ip->get_last_time_activity()) > PORT_TIMEOUT)
				this->unreserv_port(port.value);
		}
		sleep(2);
	}
}


eLockStatus 		PortController::try_reserv_port(int port, eLockStatus type_lock, std::shared_ptr<TCP_IP> tcp_ip) {
	if (!this->_map_ports.count(port)) {
		std::lock_guard<std::mutex>	gu_lock(this->_mutex);

		if (!this->_map_ports.count(port))
			this->_map_ports[port] = PortController::Port(port);
	}
	eLockStatus	status = this->_map_ports[port].lock_status;

	if (type_lock == eLockStatus::ls_LockForUse)
		return this->_persistent_reserv(port, type_lock, tcp_ip);

	if (status == eLockStatus::ls_InUse || status == eLockStatus::ls_InCheck)
		return status;
/////////////////////////////
	{
		std::lock_guard<std::mutex>	gu_lock(this->_map_ports[port].p_mutex);

		status = this->_map_ports[port].lock_status;
		if (status == eLockStatus::ls_InUse || status == eLockStatus::ls_InCheck)
			return status;

		this->_map_ports[port].lock_status = type_lock;
		this->_map_ports[port].tcp_ip = tcp_ip;
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
	this->_map_ports[port].tcp_ip = 0;
}


eLockStatus 		PortController::_persistent_reserv(int port, eLockStatus type_lock, std::shared_ptr<TCP_IP> tcp_ip) {
	std::lock_guard<std::mutex>	gu_lock(this->_map_ports[port].p_mutex);

	if (this->_map_ports[port].lock_status == ls_InUse)
		return eLockStatus::ls_InUse;
	this->_map_ports[port].lock_status = type_lock;
	this->_map_ports[port].tcp_ip = tcp_ip;
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
