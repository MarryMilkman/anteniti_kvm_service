#include "controllers/MeshConnectionController.hpp"
#include "controllers/PortController.hpp"
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

	std::vector<int> 		active_port = this->_get_active_port();
	std::vector<int>		recheck_port_list;
	if (!mesh.tcp_ip)
		mesh.tcp_ip = std::shared_ptr<TCP_IP>(new TCP_IP);

	while (active_port.size()) {
		for (int port : active_port) {
			std::string 	data_from_tunnel;
			std::string		serial_number;

			std::cerr << port << "\n";
			try {
				std::lock_guard<std::mutex>	lock(mesh.tcp_ip->s_mutex);
				if (mesh.tcp_ip->is_available) {
					// if tcp_ip is available => no need to refresh connection (it already is)
					return;
				}
				mesh.tcp_ip->fresh();
				mesh.tcp_ip->custom_connect("127.0.0.1", port);
				std::string 		message = "Command\n***DELIM***\n" SEND_MAC;

				mesh.tcp_ip->custom_write(message);
				serial_number = mesh.tcp_ip->custom_read(1);
			} catch (TCP_IP::CustomException &exception_tcp_ip) {
				if (exception_tcp_ip.type == eTypeExceptionTCP_IP::te_PortInCheck) {
					recheck_port_list.push_back(port);
					mesh.tcp_ip->is_available = false;
					mesh.tcp_ip->custom_disconnect();
					continue;
				}
				// checked port is already used some tcp_ip....
				mesh.tcp_ip->is_available = false;
				mesh.tcp_ip->custom_disconnect();
				continue;
			}
			std::cerr << serial_number << " data from tunnel.........\n";
			for (std::string sn_mesh : mesh.list_serial_number) {
				// std::cerr << sn_mesh << "\n";
				if (sn_mesh == serial_number) {
					// the desired port was found
					if (PortController::getInstance().try_reserv_port(port, eLockStatus::ls_LockForUse, mesh.tcp_ip) != eLockStatus::ls_ReservSeccess)
						break ;
					mesh.tcp_ip->is_available = true;
					mesh.tcp_ip->connected_mac = serial_number;
					return ;
				}
			}
			mesh.tcp_ip->is_available = false;
			mesh.tcp_ip->custom_disconnect();
		}
		active_port = recheck_port_list;
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
