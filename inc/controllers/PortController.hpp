#ifndef PORT_CONTROLLER_HPP
# define PORT_CONTROLLER_HPP

#include "lib.h"

class PortController {
	PortController();
public:
	~PortController();
	PortController(PortController const &ref) = delete;
	PortController operator=(PortController const & ref) = delete;
	static PortController &getInstance();

	bool 	try_reserv_port(int port);
	void 	unreserv_port(int port);

private:
	std::map<int /* port */, int /* count */>				_map_ports;
	std::map<int /* port */, std::shared_ptr<std::mutex>>	_map_mutex;
};

#endif
