#include "Tunnel.hpp"
#include "TCP_IP.hpp"

Tunnel::Tunnel() {

}

Tunnel::Tunnel(int port) {
	this->port = port;
	this->is_available = false;
	this->_tcp_ip = new TCP_IP;
	try {
		time(&this->_last_time_activity);
		this->_tcp_ip->custom_connect("127.0.0.1", port);
	} catch (std::exception &e) {
		if (this->_tcp_ip) {
			delete this->_tcp_ip;
			this->_tcp_ip = 0;
		}
		throw std::exception();
	}
}

Tunnel::~Tunnel() {
	if (this->_tcp_ip) {
		delete this->_tcp_ip;
		this->_tcp_ip = 0;
	}
}

Tunnel::Tunnel(Tunnel const &ref) {
	*this = ref;
}

Tunnel &Tunnel::operator=(Tunnel const &ref) {
	this->dest_mac = ref.dest_mac;
	this->port = ref.port;
	this->is_available = ref.is_available;
	(*this->_tcp_ip) = *ref._tcp_ip;
	return *this;
}

std::string 	Tunnel::t_read(int timeout) {
	if (!this->_tcp_ip || !this->is_available)
		throw std::exception();
	time(&this->_last_time_activity);
	try {
		return this->_tcp_ip->custom_read(timeout);
	} catch (std::exception &e) {
		this->is_available = false;
		if (this->_tcp_ip) {
			delete this->_tcp_ip;
			this->_tcp_ip = 0;
		}
		throw std::exception();
	}
}

void 			Tunnel::t_write(std::string message) {
	if (!this->_tcp_ip || !this->is_available)
		throw std::exception();
	time(&this->_last_time_activity);
	try {
		this->_tcp_ip->custom_write(message);
	} catch (std::exception &e) {
		this->is_available = false;
		if (this->_tcp_ip) {
			delete this->_tcp_ip;
			this->_tcp_ip = 0;
		}
		throw std::exception();
	}
}

time_t 			Tunnel::get_last_time_activity() const{
	return this->_last_time_activity;
}
