#include "controllers/TunnelController.hpp"
#include "Tunnel.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"

TunnelController::TunnelController() {
	this->_create_new_connection();
}

TunnelController::~TunnelController() {

}

TunnelController	&TunnelController::getInstance() {
	static TunnelController _tunnel_controller;

	return _tunnel_controller;
}



std::vector<std::shared_ptr<Tunnel>>	TunnelController::get_list_active_tunnel() {
	std::vector<std::shared_ptr<Tunnel>> 	r_list;

	for (auto item : this->_map_tunnel) {
		if (!item.second)
			continue;
		std::shared_ptr<Tunnel> &tunnel = item.second;
		if (tunnel && tunnel->is_available) {
			r_list.push_back(tunnel);
		}
	}
	return r_list;
}

void 		TunnelController::monitor_tunnels() {
	while (1) {
		this->_create_new_connection();
		this->_clean_list_tunnel();
	}
}

void 		TunnelController::_create_new_connection() {
	std::vector<int> 		active_port = this->_get_active_port();

	for (int port : active_port) {
		if (!this->_map_tunnel.count(port) || !this->_map_tunnel[port])
			this->_map_tunnel[port] = std::shared_ptr<Tunnel>(new Tunnel(port));

		// if (this->_map_tunnel.count(port) || this->_map_tunnel[port])
		// 	continue;
		// try {
		// 	this->_map_tunnel[port] = this->_create_tunnel_on(port);
		// } catch (std::exception &e) {}
	}
}

void 		TunnelController::_clean_list_tunnel() {
	for (auto &item : this->_map_tunnel) {
		std::shared_ptr<Tunnel> 	&tunnel_ptr = item.second;
		time_t 						now;

		if (!tunnel_ptr) {
			this->_map_tunnel.erase(item.first);
			continue;
		}
		time(&now);
		if (!tunnel_ptr->is_available || (now - tunnel_ptr->get_last_time_activity()) > 1100) {
			tunnel_ptr->is_available = false;
			try {
				tunnel_ptr->t_refresh_connection();
			} catch (std::exception &e) {
				tunnel_ptr = 0;
			}
		}
	}
}







std::vector<int>	TunnelController::_get_active_port() const {
	std::string 				path_to_script = ScriptExecutor::PathToScript + "detect_open_ssh_tunnel_port.sh";
	std::string 				str_with_port = ScriptExecutor::getOutput::execute(path_to_script);
	std::vector<std::string> 	list_port = Parser::custom_split(str_with_port, "\n");
	std::vector<int> 			int_list_port;

	for (std::string str : list_port) {
		try {
			int_list_port.push_back(std::stoi(str));
		} catch (std::exception &e) {}
	}
	return int_list_port;
}
