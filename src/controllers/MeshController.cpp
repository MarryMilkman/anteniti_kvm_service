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
	for (Mesh &mesh : this->_map_mesh[imei])
		if (mesh.name == name_mesh) {
			if (!mesh.tcp_ip)
				mesh.tcp_ip = std::shared_ptr<TCP_IP>(this->_mesh_connection_controller.find_connection(mesh.list_serial_number));
			return mesh;
		}
		// need_realiz
	this->_registered_new_mesh(imei);
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
	if (mesh.tcp_ip) {
		std::cerr << "detete tcp_ip in mesh\n";
		mesh.tcp_ip = 0;
	}
	mesh.tcp_ip = std::shared_ptr<TCP_IP>(this->_mesh_connection_controller.find_connection(mesh.list_serial_number));
}
