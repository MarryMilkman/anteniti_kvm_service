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

Mesh 		&MeshController::get_mesh_by(std::string serial_number) {
	std::map<std::string, std::string> 	map_imei_name;

	if (!serial_number.size())
		throw std::exception();
	std::cerr << "NEED MESH!\n";
	{
		std::unique_lock<std::mutex> 	ulock(this->_sn_mutex, std::try_to_lock);

		if (ulock.owns_lock())
			std::map<std::string, std::string> 	map_imei_name = this->_mysql_controller.get_imei_and_name_by_serial_number(serial_number);
		else
			std::lock_guard<std::mutex>		lock(this->_sn_mutex);
	}
	std::string 						imei = map_imei_name["imei"];
	std::string 						name_mesh = map_imei_name["name_mesh"];

	if (!imei.size() || !name_mesh.size())
		throw std::exception();
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
	if (this->_map_mesh[imei].count(name_mesh))
		return this->_map_mesh[imei][name_mesh];
	// for (Mesh &mesh : this->_map_mesh[imei])
	// 	if (mesh.name == name_mesh)
	// 		return mesh;
	throw std::exception();
}


//	check self list of mesh.
//		if find - check tcp_ip and fresh connect if need
//		if not_find - use _mysql_controller for find mesh in data_base (DEVICESN table) by imei + mesh
//			if found - story new mesh to _map_mesh and refresh this connection
//			if not_found - make exception
Mesh 		&MeshController::get_mesh_by(std::string imei, std::string name_mesh) {
	std::cerr << "NEED MESH!\n";
	if (!imei.size() || !name_mesh.size())
		throw std::exception();
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
	if (this->_map_mesh[imei].count(name_mesh))
		return this->_map_mesh[imei][name_mesh];
	// for (Mesh &mesh : this->_map_mesh[imei])
	// 	if (mesh.name == name_mesh)
	// 		return mesh;
	throw std::exception();
}

//	with help of _mysql_controller get list of Mesh (by imei)
//	refresh connect of this mesh and then story tham to _map_mesh
void 		MeshController::_registered_new_mesh(std::string imei) {
	std::vector<Mesh> 	list_new_mesh = Parser::MySQL::meshes_info_to_list_mesh(this->_mysql_controller.get_meshes_info_by_imei(imei));

	for (Mesh &mesh : list_new_mesh) {
		Mesh &old_mesh = this->_map_mesh[imei][mesh.name];
		if (!(old_mesh == mesh)) {
			this->refresh_connection(mesh);
			this->_map_mesh[imei][mesh.name] = mesh;
		}
	}
}


//	use _mesh_connection_controller.find_connection for refresh connection of mesh
void 		MeshController::refresh_connection(Mesh &mesh) {
	std::cerr << "refresh_connection\n";

	this->_mesh_connection_controller.find_connection(mesh);
}
