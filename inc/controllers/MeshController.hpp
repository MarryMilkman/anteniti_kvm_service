#ifndef MESH_CONTROLLER_HPP
# define MESH_CONTROLLER_HPP

#include "lib.h"

class Mesh;
class MeshConnectionController;
class MySQLController;

class MeshController {
	MeshController();
public:
	~MeshController();
	MeshController(MeshController const &ref) = delete;
	MeshController operator=(MeshController const & ref) = delete;
	static MeshController &getInstance();

	void 		operator()();

	Mesh 		&get_mesh_by(std::string imei, std::string name_mesh);
	Mesh 		&get_mesh_by(std::string serial_number);
	void 		refresh_connection(Mesh &mesh);

private:
	std::mutex 					_sn_mutex;
	MySQLController				&_mysql_controller;
	MeshConnectionController	&_mesh_connection_controller;

	std::map<std::string  /* imei */, std::map<std::string  /* name */, Mesh>>	_map_mesh;
	std::map<std::string /* imei */, std::shared_ptr<std::mutex>>				_map_mutex;

	void 						_registered_new_mesh(std::string imei);
};

#endif
