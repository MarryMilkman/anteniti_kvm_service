#ifndef MESH_HPP
#define MESH_HPP

#include "lib.h"

class 	TCP_IP;

class Mesh {
public:
	Mesh();
	~Mesh();
	Mesh(Mesh const & ref);
	Mesh &operator=(Mesh const & ref);

	std::string 				imei;
	std::string 				name;
	std::vector<std::string>	list_serial_number;
	std::shared_ptr<TCP_IP>		tcp_ip;
	// std::vector<std::string> 	&get_serial_numbers() const;
	// void 						add_serial_number(std::string serial_number);

	// TCP_IP_Worker 				*get_tcp_ip();
	// void 						refresh_tcp_ip(TCP_IP_Worker *new_tcp_ip);
};

#endif
