#ifndef PORT_CONTROLLER_HPP
# define PORT_CONTROLLER_HPP

#include "lib.h"

enum eLockStatus {
	ls_ReservSeccess = 0,
	ls_InCheck,
	ls_InUse,
	ls_LockForCheck,
	ls_LockForUse,
	ls_Free
};

class PortController {
	PortController();

///// Port
	class Port {
	public:
		Port();
		Port(int port);
		Port(Port const &ref);
		Port &operator=(Port const &ref);
		~Port();

		int 		value;
		eLockStatus	lock_status;
		std::mutex	p_mutex;
	};
////////

public:
	~PortController();
	PortController(PortController const &ref) = delete;
	PortController operator=(PortController const & ref) = delete;
	static PortController &getInstance();

	eLockStatus 	try_reserv_port(int port, eLockStatus type_lock);
	void 			unreserv_port(int port);



private:
	std::map<int /* number port */, PortController::Port>	_map_ports;
	std::mutex 												_mutex;
	// std::map<int /* port */, std::shared_ptr<std::mutex>>	_map_mutex;

	eLockStatus 			_persistent_reserv(int port, eLockStatus type_lock);


};

#endif
