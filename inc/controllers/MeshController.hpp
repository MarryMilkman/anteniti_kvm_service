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
	void 		refresh_connection(Mesh &mesh);

private:
	MySQLController								&_mysql_controller;
	MeshConnectionController					&_mesh_connection_controller;
	std::map<std::string, std::vector<Mesh>>	_map_mesh;

	void 						_registered_new_mesh(std::string imei);
};

#endif
