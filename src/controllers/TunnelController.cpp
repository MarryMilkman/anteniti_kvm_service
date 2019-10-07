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
		if (item.second->is_available) {
			r_list.push_back(item.second);
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
		if (this->_map_tunnel.count(port) || this->_map_tunnel[port])
			continue;
		try {
			this->_map_tunnel[port] = this->_create_tunnel_on(port);
		} catch (std::exception &e) {}
	}
}

void 		TunnelController::_clean_list_tunnel() {
	for (auto &item : this->_map_tunnel) {
		std::shared_ptr<Tunnel> 	&tunnel_ptr = item.second;
		time_t 						now;

		if (!tunnel_ptr || !tunnel_ptr->is_available) {
			this->_map_tunnel.erase(item.first);
			continue;
		}
		time(&now);
		if ((now - tunnel_ptr->get_last_time_activity()) > 1100) {
			int port = tunnel_ptr->port;

			std::cerr << "hi!\n";
			tunnel_ptr->is_available = false;
			tunnel_ptr = 0;
			try {
				tunnel_ptr = this->_create_tunnel_on(port);
			} catch (std::exception &e) {
				tunnel_ptr = 0;
			}
		}
	}
}




std::shared_ptr<Tunnel> 	TunnelController::_create_tunnel_on(int port) {
	Tunnel 						*tunnel = new Tunnel(port);
	std::string 				message = "Command\n***DELIM***\n" SEND_MAC;
	int 						timeout = 2;
	std::string 				answer;
	std::lock_guard<std::mutex>	lock(tunnel->s_mutex);

	tunnel->dest_mac = "";
	tunnel->is_available = true;
	tunnel->t_write(message);
	answer = tunnel->t_read(timeout);
	if (!answer.size() /*|| this->_is_answer_correct(answer)*/)
		std::exception();
	tunnel->dest_mac = answer;
	tunnel->is_available = true;
	return std::shared_ptr<Tunnel>(tunnel);
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
