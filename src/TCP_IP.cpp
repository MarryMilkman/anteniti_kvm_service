#include "TCP_IP.hpp"

TCP_IP::TCP_IP() {
	this->_socket = 0;
	this->fresh();
}

TCP_IP::~TCP_IP() {
	this->fresh();
}

TCP_IP::TCP_IP(TCP_IP const & ref) {
	*this = ref;
}

TCP_IP	&TCP_IP::operator=(TCP_IP const & ref) {
	this->_socket = ref.get_socket();
	return *this;
}

	// connect
void 		TCP_IP::custom_connect(std::string ip, int port) {
	this->_init(ip, port);
	fd_set          readfds;
    struct timeval  timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(this->_socket , &readfds);
    int activity = select(this->_socket + 1, &readfds, 0, 0, &timeout);
    if (activity <= 0) {
        throw std::exception();
    }
    if (connect(this->_socket, (struct sockaddr *)&this->_addres, sizeof(this->_addres)) < 0)
        throw std::exception();
}

	// read
std::string	TCP_IP::custom_read() {
	char	    buffer[64];
    std::string message;
    int         byts;

	while ((byts = recv(this->_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[byts] = 0;;
        message += buffer;
        buffer[0] = 0;
    }
    // shutdown(this->_socket, 0);
	if (byts < 0)
		throw std::exception();
	return message;
}

	// write
void 		TCP_IP::custom_write(std::string message) {
	int 		byts;
    const char	*buff;
    int         size = 0;

    buff = message.c_str();
    while (buff[size])
		size++;
	if (!size)
		return;
    while ((byts = send(this->_socket, buff, size, 0)) > 0) {
        if (byts >= size)
			break;
		size -= byts;
		buff += byts;
    }
	if (byts < 0)
		throw std::exception();
    // shutdown(this->_socket, 1);
}

	// disconnect
void 		TCP_IP::custom_disconnect() {
	if (this->_socket > 0)
		close(this->_socket);
	this->_socket = 0;
}


void 		TCP_IP::_init(std::string ip, int port) {
	int     opt = 1;

	if (this->_socket > 0) {
		close(this->_socket);
		this->_socket = 0;
	}
	if (!(this->_socket = socket(AF_INET, SOCK_STREAM, 0)))
        throw std::exception();
	setsockopt(this->_socket, SOL_SOCKET, SO_DONTROUTE, &opt, sizeof(opt));
	this->_addres.sin_family = AF_INET;
	this->_addres.sin_port = htons(port);
	this->_addres.sin_addr.s_addr = inet_addr(ip.c_str());
	memset(this->_addres.sin_zero, '\0', sizeof(this->_addres.sin_zero));
}

	// fresh()
void 		TCP_IP::fresh() {
	if (this->_socket)
		close (this->_socket);
	this->_socket = 0;
}

int 		TCP_IP::get_socket() const {
	return this->_socket;
}
