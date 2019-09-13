#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "lib.h"

class Client
{
	Client() = delete;
public:
	Client(std::string host, int port);
	~Client();

private:
	int 								_port;
	std::string							_host;

	int									_sock_fd;
	struct sockaddr_in					_serv_addr;

	void 			_init_host_port(std::string host, int port);
	void 			_init_sock_and_connect();

	void			_startWork();

	void			_send_request(std::string request);
	std::string		_get_response();
};

#endif
