#ifndef TCP_IP_HPP
#define TCP_IP_HPP

#include "lib.h"

class TCP_IP {
public:
	TCP_IP();
	~TCP_IP();
	TCP_IP(TCP_IP const &ref);
	TCP_IP &operator=(TCP_IP const &ref);

	void 		custom_connect(std::string ip, int port);
	std::string	custom_read(int timeout);
	void 		custom_write(std::string message);
	void 		custom_disconnect();

	int 		get_socket() const;
	void 		fresh();

	std::mutex 		s_mutex;

	int 			status;

private:
	int 				_socket;
	struct sockaddr_in	_addres;
	int 				_port;

	void 				_init(std::string ip, int port);

};

#endif
