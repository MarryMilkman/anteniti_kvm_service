#include "Mesh.hpp"
#include "controllers/MeshConnectionController.hpp"
#include "TCP_IP.hpp"

Mesh::Mesh() {
	std::cerr << "Create new MESH!\n";
	this->tcp_ip = 0;
}

Mesh::~Mesh() {
	// if (this->tcp_ip)
	this->tcp_ip = 0;
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

bool 	Mesh::operator==(Mesh const & ref) {
	if (this->name != ref.name || this->imei != ref.imei ||
			this->list_serial_number.size() != this->list_serial_number.size())
		return false;
	for (std::string currennt_sn : this->list_serial_number) {
		bool 	is_exist = false;

		for (std::string cmp_sn : ref.list_serial_number)
			if (currennt_sn == cmp_sn)
				is_exist = true;
		if (!is_exist)
			return false;
	}
	return true;
}

void 		Mesh::refresh_connection() {
	MeshConnectionController::getInstance().find_connection(*this);
}
