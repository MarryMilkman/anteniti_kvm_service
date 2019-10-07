#ifndef TUNNEL_CONTROLLER_HPP
# define TUNNEL_CONTROLLER_HPP

# include "lib.h"

class Tunnel;

class TunnelController {
	TunnelController();
public:
	~TunnelController();
	TunnelController(TunnelController const &ref) = delete;
	TunnelController &operator=(TunnelController const &ref) = delete;
	static TunnelController &getInstance();

	void 										monitor_tunnels();
	std::vector<std::shared_ptr<Tunnel>> 		get_list_active_tunnel();

private:
	std::map<int /* port */, std::shared_ptr<Tunnel>>	_map_tunnel;

	void 						_create_new_connection();
	void 						_clean_list_tunnel();
	std::shared_ptr<Tunnel> 	_create_tunnel_on(int port);
	std::vector<int>			_get_active_port() const;


};

#endif
