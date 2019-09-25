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
class Mesh;

class MeshConnectionController {
	MeshConnectionController();
public:
	~MeshConnectionController();
	MeshConnectionController(MeshConnectionController const &ref) = delete;
	MeshConnectionController &operator=(MeshConnectionController const &ref) = delete;
	static MeshConnectionController &getInstance();

		// for SSHTunnelDataController
		// SSHTunnelData 	&get_SSHTunnelData_by_sn(std::string serial_number) const;
	void	find_connection(Mesh &mesh);

private:

	std::vector<int>		_get_active_port() const;
};

#endif
