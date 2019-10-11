#include "Tunnel.hpp"
#include "TCP_IP.hpp"

Tunnel::Tunnel() {

}

Tunnel::Tunnel(int port) {
	this->_port = port;
	this->is_available = false;
	this->_tcp_ip = 0;
	this->t_refresh_connection();
	std::cerr << "Tunnel create!!!!!!!!!!!!!!!!\n";
}

Tunnel::~Tunnel() {
	std::stringstream 	ss;

	ss << this << " dellite tunnel\n";
	std::cerr << ss.str();
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
	this->_port = ref._port;
	this->is_available = ref.is_available;
	this->_tcp_ip = ref._tcp_ip;
	return *this;
}

void 			Tunnel::t_refresh_connection() {
	std::unique_lock<std::mutex> lock(this->connect_mutex, std::try_to_lock);

	if (!lock.owns_lock()) {
		std::lock_guard<std::mutex> llock(this->connect_mutex);
		return ;
	}


	if (!this->_tcp_ip)
		this->_tcp_ip = new TCP_IP;
	else
		this->_tcp_ip->fresh();
	time(&this->_last_time_activity);
	this->is_available = false;
	this->dest_mac = "";
	try {
		this->_tcp_ip->custom_connect("127.0.0.1", this->_port);
		// this->is_available = true;
	} catch (std::exception &e) {
		this->_tcp_ip->fresh();
			//only if connection failed!
		throw std::exception();
	}
	if (!this->_find_dest_mesh())
		this->_tcp_ip->fresh();
}

bool 			Tunnel::_find_dest_mesh() {
	std::string 				message = "Command\n***DELIM***\n" SEND_MAC;
	int 						timeout = 2;
	std::string 				answer;
	std::lock_guard<std::mutex>	lock(this->rw_mutex);

	this->dest_mac = "";
	this->is_available = true;
	try {
		this->t_write(message);
		answer = this->t_read(timeout);
	} catch (std::exception &e) {}
	if (!answer.size() /*|| this->_is_answer_correct(answer)*/) {
		this->is_available = false;
		return false;
	}
	this->dest_mac = answer;
	this->is_available = true;
	std::cerr << "_find_dest_mesh TRUE!\n";
	return true;
}



std::string 	Tunnel::t_read(int timeout) {
	if (!this->_tcp_ip || !this->is_available)
		throw std::exception();
	time(&this->_last_time_activity);
	try {
		return this->_tcp_ip->custom_read(timeout);
	} catch (std::exception &e) {
		this->is_available = false;
		// if (this->_tcp_ip) {
		// 	delete this->_tcp_ip;
		// 	this->_tcp_ip = 0;
		// }
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
		// if (this->_tcp_ip) {
		// 	delete this->_tcp_ip;
		// 	this->_tcp_ip = 0;
		// }
		throw std::exception();
	}
}

time_t 			Tunnel::get_last_time_activity() const{
	return this->_last_time_activity;
}
