#ifndef TCP_IP_HPP
#define TCP_IP_HPP

#include "lib.h"

enum eTypeExceptionTCP_IP {
	te_PortInCheck = 0,
	te_PortInUse,
	te_SysError
};


class TCP_IP {
public:

	class CustomException: public std::exception
	{
		CustomException() throw();
	public:
		~CustomException() throw();
		CustomException(eTypeExceptionTCP_IP exceptType) throw();
		CustomException(CustomException const & ref) throw();
		CustomException &operator=(CustomException const & ref) throw();

	    // virtual const char		*what() const throw();
		eTypeExceptionTCP_IP	type;
	};



	TCP_IP();
	~TCP_IP();
	TCP_IP(TCP_IP const &ref);
	TCP_IP &operator=(TCP_IP const &ref);

	time_t 		get_last_time_activity();

	void 		custom_connect(std::string ip, int port);
	std::string	custom_read(int timeout);
	void 		custom_write(std::string message);
	void 		custom_disconnect();

	int 		get_socket() const;
	void 		fresh();

	std::mutex 		s_mutex;
	std::string 	connected_mac;
	bool 			is_available;

private:
	int 				_socket;
	struct sockaddr_in	_addres;
	int 				_port;
	time_t 				_last_time_activity;

	void 				_init(std::string ip, int port);

};

#endif
