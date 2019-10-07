#include "Mesh.hpp"
#include "controllers/TunnelController.hpp"
#include "Tunnel.hpp"

Mesh::Mesh() :
	tunnel(0)
{
	std::cerr << "Create new MESH!\n";
}

Mesh::~Mesh() {
	this->tunnel = 0;
}

Mesh::Mesh(Mesh const & ref) {
	*this = ref;
}

Mesh 	&Mesh::operator=(Mesh const & ref) {
	this->tunnel = ref.tunnel;
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

void		Mesh::refresh_connection() {
	std::unique_lock<std::mutex> lock(this->_refresh_connection_mutex, std::try_to_lock);

	if (!lock.owns_lock()) {
		std::lock_guard<std::mutex> llock(this->_refresh_connection_mutex);
		return ;
	}
	if (!this->list_serial_number.size())
		return ;
	std::vector<std::shared_ptr<Tunnel>>	list_active_tunnel = TunnelController::getInstance().get_list_active_tunnel();

	this->tunnel = 0;
	// std::cerr << list_active_tunnel.size() << " active port\n";
	for (std::shared_ptr<Tunnel> tunnel : list_active_tunnel) {
		// std::cerr << "tunnel->dest_mac " << tunnel->dest_mac << "\n";
		if (tunnel->is_available) {
			for (std::string sn_mesh : this->list_serial_number) {
				if (tunnel->dest_mac == sn_mesh) {
					std::cerr << sn_mesh << " - find mesh\n";
					this->tunnel = tunnel;
					return;
				}
			}
		}
	}
}
