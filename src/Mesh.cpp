#include "Mesh.hpp"
#include "TCP_IP.hpp"

Mesh::Mesh() {
	this->tcp_ip = 0;
}

Mesh::~Mesh() {
	if (this->tcp_ip)
		delete this->tcp_ip;
}

Mesh::Mesh(Mesh const & ref) {
	*this = ref;
}

Mesh 	&Mesh::operator=(Mesh const & ref) {
	this->tcp_ip = ref.tcp_ip;
	this->list_serial_number.clear();
	for (std::string sn : ref.list_serial_number)
		this->list_serial_number.push_back(sn);
	this->name = ref.name;
	this->imei = ref.imei;
	return *this;
}
