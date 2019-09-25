#include "controllers/MeshConnectionController.hpp"
#include "ScriptExecutor.hpp"
#include "TCP_IP.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"

MeshConnectionController::MeshConnectionController()
{
}

MeshConnectionController::~MeshConnectionController() {

}

MeshConnectionController	&MeshConnectionController::getInstance() {
	static MeshConnectionController mesh_connection_controller;

	return mesh_connection_controller;
}

// check open port and compare data from tunnel with std::vector; stroy if find equal
std::shared_ptr<TCP_IP>		MeshConnectionController::find_connection(std::vector<std::string> &serial_numbers_of_mesh) {
	std::cerr << "mesh_connection_controller.find_connection\n";
	if (!serial_numbers_of_mesh.size())
		return 0;

	std::vector<int> 		active_port = this->_get_active_port();
	std::shared_ptr<TCP_IP> tcp_ip(new TCP_IP);

	for (int port : active_port) {
		std::string 	data_from_tunnel;
		std::string		serial_number;

		try {
			std::lock_guard<std::mutex> lock(tcp_ip->s_mutex);
			tcp_ip->custom_connect("127.0.0.1", port);
			std::string 		message = "Command\n***DELIM***\n" SEND_MAC;

			tcp_ip->custom_write(message);
			// data_from_tunnel = tcp_ip->custom_read();
			// serial_number = Parser::SSHTunnel::get_serial_number_from_authorization(data_from_tunnel);
			serial_number = tcp_ip->custom_read();
			std::cerr << serial_number << "\n";
		} catch (std::exception &e) {
			std::cerr << "ASHIBKA!\n";
			tcp_ip->custom_disconnect();
			continue;
		}
		std::cerr << serial_number << " data from tunnel.........\n";
		for (std::string sn_mesh : serial_numbers_of_mesh) {
			if (sn_mesh == serial_number) {
				return tcp_ip;
			}
		}
		tcp_ip->custom_disconnect();
		// tcp_ip->fresh();
	}
	tcp_ip = 0;
	return 0;
}

std::vector<int>	MeshConnectionController::_get_active_port() const {
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


////////
