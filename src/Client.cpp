#include "Client.hpp"
#include "controllers/CloudController.hpp"
#include "ScriptExecutor.hpp"

Client::Client(std::string host, int port)
{
	signal(SIGPIPE, SIG_IGN);
	this->_init_host_port(host, port);
	this->_startWork();
}

Client::~Client(){
	if (this->_sock_fd)
		close(this->_sock_fd);
}

void Client::_init_host_port(std::string host, int port) {
	this->_host = host;
	this->_port = port;
}

void 			Client::_init_sock_and_connect() {
    int     					opt = 1;
	std::string 				str_with_port = ScriptExecutor::getOutput::execute(1, "../scripts/detect_open_ssh_tunnel_port.sh");
	std::vector<std::string> 	list_port = custom_split(str_with_port, "\n");
	int 						port = -1;

	if (!list_port.size())
		throw std::exception();
	std::cerr << list_port.size() << "...\n";
	for (unsigned i = 0; i < list_port.size() && port < 0; ++i) {
		try {
			std::cerr << i << "?\n";
			port = stoi(list_port[i]);
		} catch (std::exception const & ref) {}
	}
	if (port == -1)
		std::exception();
	if (!(this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw std::exception();
	bzero((char *) &this->_serv_addr, sizeof(this->_serv_addr));
	setsockopt(this->_sock_fd, SOL_SOCKET, SO_DONTROUTE, &opt, sizeof(opt));
    // setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    this->_serv_addr.sin_family = AF_INET;
    // this->_serv_addr.sin_port = htons(port);
    this->_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
	this->_serv_addr.sin_port = htons(port);
	connect(this->_sock_fd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr));
	if (this->_sock_fd < 0) {
		std::cerr << port << " connect failed\n";
		throw std::exception();
	}
	std::cerr << port << " find port!\n";
}

void			Client::_startWork() {
	std::string		line;
	CloudController &cloud_controller = CloudController::getInstance();
	std::string		response;
	int 			i = 0;

	this->_init_sock_and_connect();
	std::cerr << "Connect to: " << inet_ntoa(this->_serv_addr.sin_addr) << ":" << this->_serv_addr.sin_port << "\n";
	response = this->_get_response();
	std::cout << "Response:\n" << response << "\n\n";
	while (i < 6) {
		cloud_controller.get_setting_from_cloud();
		std::fstream	send_file;
		std::string		send_str = "";

		send_file.open(PATH_VARIABLE_SETTING);
		send_str = SETTING_CHENGED;
		send_str += "\n***DELIM***\n";
		while (getline(send_file, line))
			send_str += line + "\n";
		this->_send_request(send_str);
		sleep(3);
		if (!(i % 2)) {
			std::cerr << "send info\n";
			line = SEND_INFO;
			if (i == 4)
				line += "\n****DELIM****\n";
			else
				line += "\n***DELIM***\n";
			line += "bla bla\n";
			std::cerr << line << "\n-----------------\n";
			this->_send_request(line);
		}
		std::cerr << "------------\n\n";
		sleep(5);
		i++;
	}
	std::cerr << "Poka!\n";
}

void			Client::_send_request(std::string line) {
	if (send(this->_sock_fd, line.c_str(), line.size(), 0) < 0) {
		std::cerr << "sock disconnect\n";
		std::exception();
	}
	// shutdown(this->_sock_fd, 1);
}

std::string		Client::_get_response() {
	char						buf[500000];
	int 						val = 0;
	int							bytes = 0;

	val = read(this->_sock_fd, buf, 1024);
	bytes += val;

	buf[bytes] = 0;
	return buf;
}
