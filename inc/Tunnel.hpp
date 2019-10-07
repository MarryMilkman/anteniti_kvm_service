#ifndef TUNNEL_HPP
# define TUNNEL_HPP

# include "lib.h"

class TCP_IP;

class Tunnel {
	Tunnel();
public:
	~Tunnel();
	Tunnel(int port);
	Tunnel(Tunnel const &ref);
	Tunnel &operator=(Tunnel const &ref);

	std::string	t_read(int timeout);
	void 		t_write(std::string message);

	time_t 		get_last_time_activity() const;

	int 		port;
	std::mutex 	s_mutex;
	bool 		is_available;
	std::string dest_mac;

private:
	TCP_IP		*_tcp_ip;
	time_t 		_last_time_activity;

};

#endif
