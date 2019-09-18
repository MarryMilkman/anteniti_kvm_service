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

// // operator for work in thread;
// void 		MeshController::operator()() {
// 	while (1) {
// 		this->_registered_new_mesh();
// 		this->_registered_new_routers();
// 	}
// }

// void 		MeshController::_registered_new_mesh() {
// 	std::vector<MySQLDataSegment *> &list_data = this->_mysql_controller.get_request(rd_NewMeshRequest);
// 	std::vector<Mesh> 							list_unregistred_mesh;
//
// 	for (MySQLDataSegment *data_segment : list_data) {
// 		Mesh 	new_mesh;
// 		for (std::string)
// 		..................................................
// 		....................................................
// 		.......................................
// 		.............................................
// 	}
// 	if (!list_unregistred_mesh.size())
// 		return;
// 	for (Mesh new_mesh : list_unregistred_mesh) {
// 		TCP_IP *tcp_ip = this->_mesh_connection_controller.find_connection(new_mesh.list_serial_number);
//
// 		if (tcp_ip) {
// 			new_mesh.tcp_ip = tcp_ip;
// 			this->_mysql_controller.registered_mesh(new_mesh);
// 		}
// 		else {
// 			this->_mysql_controller.add_error(new_mesh, "No ssh tunnel");
// 		}
// 	}
// }

// void 		MeshController::_registered_new_routers() {
// 	std::vector<std::string>	&list_unregistred_routers = this->_mysql_controller.get_list_unregistred_routers();
//
// 	if (!_list_unregistred_routers.size())
// 		return;
// 	for (std::string str_router_info : list_unregistred_routers) {
// 		std::vector<std::string>	segment_info_routers = Parser::custom_split(str_router_info, " ");
// 		bool 						is_find = false;
//
// 		for (Mesh &mesh : this->_list_mesh) {
// 			for (std::string sn : mesh.list_serial_number) {
// 				if (sn == segment_info_routers[0]) {
// 					mesh.list_serial_number.push_back(segment_info_routers[1]);
// 					is_find = true;
// 					break;
// 				}
// 			}
// 			if (is_find)
// 				break;
// 		}
// 		if (!is_find) {
// 			this->_mysql_controller.add_error(segment_info_routers[1], "No register mesh for this routers");
// 		}
// 	}
// }


Mesh 		&MeshController::get_mesh_by(std::string imei, std::string name_mesh) {

	for (Mesh &mesh : this->_map_mesh[imei])
		if (mesh.name == name_mesh)
			return mesh;
		// need_realiz
	this->_registered_new_mesh(imei);
	for (Mesh &mesh : this->_map_mesh[imei])
		if (mesh.name == name_mesh)
			return mesh;
	throw std::exception();
}

void 		MeshController::_registered_new_mesh(std::string imei) {
	std::vector<Mesh> 	list_new_mesh = Parser::MySQL::meshes_info_to_list_mesh(this->_mysql_controller.get_meshes_info_by_imei(imei));
	Mesh 				r_mesh;

	for (Mesh &mesh : list_new_mesh) {
		this->_map_mesh[imei].push_back(mesh);
		this->refresh_connection(mesh);
	}
}



void 		MeshController::refresh_connection(Mesh &mesh) {
	if (mesh.tcp_ip) {
		delete mesh.tcp_ip;
		mesh.tcp_ip = 0;
	}
	mesh.tcp_ip = this->_mesh_connection_controller.find_connection(mesh.list_serial_number);
}
