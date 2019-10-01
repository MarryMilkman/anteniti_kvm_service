#include "controllers/MeshConnectionController.hpp"
#include "ScriptExecutor.hpp"
#include "TCP_IP.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"
#include "Mesh.hpp"

MeshConnectionController::MeshConnectionController()
{
}

MeshConnectionController::~MeshConnectionController() {

}

MeshConnectionController	&MeshConnectionController::getInstance() {
	static MeshConnectionController mesh_connection_controller;

	return mesh_connection_controller;
}

// 	check open port and compare data from tunnel with std::vector of serial_number;
//		story std::shared_ptr<TCP_IP> to mesh.tcp_ip if find equal
void		MeshConnectionController::find_connection(Mesh &mesh) {
	std::cerr << "mesh_connection_controller.find_connection\n";
	std::unique_lock<std::mutex> lock(mesh.refresh_connection_mutex, std::try_to_lock);

	if (!lock.owns_lock()) {
		std::lock_guard<std::mutex> llock(mesh.refresh_connection_mutex);
		return ;
	}
	if (!mesh.list_serial_number.size())
		return ;
	// std::unique_lock<std::mutex> un_lock(mesh.refresh_connection_mutex, std::try_to_lock);
	// if (!un_lock.owns_lock()) {
	// 	std::lock_guard<std::mutex>	lg_lock(mesh.refresh_connection_mutex);
	// 	return ;
	// }

	std::vector<int> 		active_port = this->_get_active_port();
	if (!mesh.tcp_ip)
		mesh.tcp_ip = std::shared_ptr<TCP_IP>(new TCP_IP);

	for (int port : active_port) {
		std::string 	data_from_tunnel;
		std::string		serial_number;

		try {
			std::lock_guard<std::mutex>	lock(mesh.tcp_ip->s_mutex);
			if (!mesh.tcp_ip->status)
				return;
			mesh.tcp_ip->fresh();
			mesh.tcp_ip->custom_connect("127.0.0.1", port);
			std::string 		message = "Command\n***DELIM***\n" SEND_MAC;

			mesh.tcp_ip->custom_write(message);
			// data_from_tunnel = tcp_ip->custom_read();
			// serial_number = Parser::SSHTunnel::get_serial_number_from_authorization(data_from_tunnel);
			serial_number = mesh.tcp_ip->custom_read(1);
			std::cerr << serial_number << "\n";
		} catch (std::exception &e) {
			std::stringstream 	print_ss;

			mesh.tcp_ip->status = 1;
			print_ss << port << " - Uncorrect port for connect......error\n";
			mesh.tcp_ip->custom_disconnect();
			continue;
		}
		std::cerr << serial_number << " data from tunnel.........\n";
		for (std::string sn_mesh : mesh.list_serial_number) {
			if (sn_mesh == serial_number) {
				mesh.tcp_ip->status = 0;
				mesh.tcp_ip->connected_mac = serial_number;
				return ;
			}
		}
		mesh.tcp_ip->status = 1;
		mesh.tcp_ip->custom_disconnect();
		// tcp_ip->fresh();
	}
}

// use scrept for check open port with LISTEN on addr 127.0.0.1; return list of find ports
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
