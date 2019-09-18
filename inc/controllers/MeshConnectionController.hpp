#ifndef MESH_CONNECTION_CONTROLLER
# define MESH_CONNECTION_CONTROLLER

# include "lib.h"

enum eStatus{
	e_ok = 0,
	eError_,
	eError_noConnection,
	eError_failConnection
};

class TCP_IP;

class MeshConnectionController {
	MeshConnectionController();
public:
	~MeshConnectionController();
	MeshConnectionController(MeshConnectionController const &ref) = delete;
	MeshConnectionController &operator=(MeshConnectionController const &ref) = delete;
	static MeshConnectionController &getInstance();

		// for SSHTunnelDataController
		// SSHTunnelData 	&get_SSHTunnelData_by_sn(std::string serial_number) const;
	TCP_IP 		*find_connection(std::vector<std::string> &serial_numbers_of_mesh);

private:

	std::vector<int>		_get_active_port() const;
};

#endif
