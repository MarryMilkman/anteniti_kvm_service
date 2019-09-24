#include "controllers/MeshController.hpp"
#include "controllers/MeshConnectionController.hpp"
#include "controllers/MySQLController.hpp"
#include "Mesh.hpp"
#include "TCP_IP.hpp"
#include "Parser.hpp"

MeshController::MeshController() :
	_mysql_controller(MySQLController::getInstance()),
	_mesh_connection_controller(MeshConnectionController::getInstance())
{
}

MeshController::~MeshController() {

}

MeshController 	&MeshController::getInstance() {
	static MeshController	_mesh_controller;

	return _mesh_controller;
}

Mesh 		&MeshController::get_mesh_by(std::string imei, std::string name_mesh) {
	std::cerr << "NEED MESH!\n";
	for (Mesh &mesh : this->_map_mesh[imei])
		if (mesh.name == name_mesh) {
			if (!mesh.tcp_ip)
				mesh.tcp_ip = std::shared_ptr<TCP_IP>(this->_mesh_connection_controller.find_connection(mesh.list_serial_number));
			return mesh;
		}
		// need_realiz
	if (!_map_mutex.count(imei)) {
		_map_mutex[imei] = std::shared_ptr<std::mutex>(new std::mutex());
	}

	{
		std::unique_lock<std::mutex> 	ulock(*this->_map_mutex[imei], std::try_to_lock);

		if (ulock.owns_lock())
			this->_registered_new_mesh(imei);
		else
			std::lock_guard<std::mutex>		lock(*this->_map_mutex[imei]);
	}
	for (Mesh &mesh : this->_map_mesh[imei])
		if (mesh.name == name_mesh)
			return mesh;
	throw std::exception();
}

void 		MeshController::_registered_new_mesh(std::string imei) {
	std::vector<Mesh> 	list_new_mesh = Parser::MySQL::meshes_info_to_list_mesh(this->_mysql_controller.get_meshes_info_by_imei(imei));

	for (Mesh &mesh : list_new_mesh)
		this->refresh_connection(mesh);
	this->_map_mesh[imei] = list_new_mesh;
}



void 		MeshController::refresh_connection(Mesh &mesh) {
	std::cerr << "refresh_connection\n";
	std::unique_lock<std::mutex> lock(mesh.refresh_connection_mutex, std::try_to_lock);

	if (!lock.owns_lock()) {
		std::lock_guard<std::mutex> llock(mesh.refresh_connection_mutex);
		return ;
	}
	if (mesh.tcp_ip) {
		std::cerr << "detete tcp_ip in mesh\n";
		mesh.tcp_ip = 0;
	}
	std::cerr << "salom\n";
	mesh.tcp_ip = std::shared_ptr<TCP_IP>(this->_mesh_connection_controller.find_connection(mesh.list_serial_number));
}
