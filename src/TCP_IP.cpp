#include "TCP_IP.hpp"
#include "controllers/PortController.hpp"

TCP_IP::TCP_IP() {
	this->_port = 0;
	this->_socket = 0;
	this->is_available = false;
	std::stringstream	ss;

	ss << this << " New TCP_IP created\n";
	std::cerr << ss.str();
	this->fresh();
}

TCP_IP::~TCP_IP() {
	std::stringstream	ss;

	PortController::getInstance().unreserv_port(this->_port);
	ss << this << " Delete tcp_ip\n";
	std::cerr << ss.str();
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
	this->_port = port;
	std::stringstream	ss;
	eLockStatus 		lock_status;

	ss << this << " ..................connect..................\n";
	std::cerr << ss.str();
	lock_status = PortController::getInstance().try_reserv_port(port, eLockStatus::ls_LockForCheck);

	if (lock_status != eLockStatus::ls_ReservSeccess) {
		if (lock_status == eLockStatus::ls_InCheck)
			throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_PortInCheck);
		else if (lock_status == eLockStatus::ls_InUse)
			throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_PortInUse);
		else
			throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
	}
	this->_init(ip, port);
	fd_set          readfds;
    struct timeval  timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(this->_socket , &readfds);
    int activity = select(this->_socket + 1, &readfds, 0, 0, &timeout);
    if (activity <= 0) {
        throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
    }
    if (connect(this->_socket, (struct sockaddr *)&this->_addres, sizeof(this->_addres)) < 0)
        throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
}

	// read
std::string	TCP_IP::custom_read(int timeout) {
	std::stringstream	ss;

	ss << this << " ..................read..................\n";
	std::cerr << ss.str();

	char	    buffer[124];
    std::string message;
    int         byts;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	setsockopt(this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	byts = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
	if (byts < 0)
		throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
    buffer[byts] = 0;
    message = buffer;
    // shutdown(this->_socket, 0);
	// shutdown(this->_socket, 0);
	return message;
}

	// write
void 		TCP_IP::custom_write(std::string message) {
	std::stringstream	ss;

	ss << this << " ..................write..................\n";
	std::cerr << ss.str();

	int 		byts;
    const char	*buff;
    int         size = 0;

    buff = message.c_str();
    while (buff[size])
		size++;
	if (!size)
		return;
    while ((byts = send(this->_socket, buff, size, MSG_NOSIGNAL)) > 0) {
        if (byts >= size)
			break;
		size -= byts;
		buff += byts;
    }
	if (byts < 0) {
		std::stringstream	ss;

		ss << this << " HUETA!\n";
		std::cerr << ss.str();
		throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
	}
    // shutdown(this->_socket, 1);
}

	// disconnect
void 		TCP_IP::custom_disconnect() {
	std::stringstream	ss;

	ss << this << "  ..................disconnect from port:" << this->_port << "\n";
	std::cerr << ss.str();
	PortController::getInstance().unreserv_port(this->_port);

	if (this->_socket > 0)
		close(this->_socket);
	this->_socket = 0;
}


void 		TCP_IP::_init(std::string ip, int port) {
	int     opt = 1;

	std::stringstream	ss;

	ss << this << " ..................init..................\n";
	std::cerr << ss.str();

	if (this->_socket > 0) {
		close(this->_socket);
		this->_socket = 0;
	}
	if (!(this->_socket = socket(AF_INET, SOCK_STREAM, 0)))
        throw TCP_IP::CustomException(eTypeExceptionTCP_IP::te_SysError);
	// fcntl(this->_socket, F_SETFL, O_NONBLOCK);
	// setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &n_opt, sizeof(n_opt));
	setsockopt(this->_socket, SOL_SOCKET, SO_DONTROUTE, &opt, sizeof(opt));
	this->_addres.sin_family = AF_INET;
	this->_addres.sin_port = htons(port);
	this->_addres.sin_addr.s_addr = inet_addr(ip.c_str());
	memset(this->_addres.sin_zero, '\0', sizeof(this->_addres.sin_zero));
}

	// fresh()
void 		TCP_IP::fresh() {

	std::stringstream	ss;

	ss << this << " ..................fresh..................\n";
	std::cerr << ss.str();
	PortController::getInstance().unreserv_port(this->_port);
	this->_port = 0;
	if (this->_socket)
		close (this->_socket);
	this->_socket = 0;
}

int 		TCP_IP::get_socket() const {
	return this->_socket;
}







/////// class CustomException



TCP_IP::CustomException::CustomException() throw()
{
}

TCP_IP::CustomException::CustomException(CustomException const & ref) throw()
{
	*this = ref;
}

TCP_IP::CustomException::CustomException(eTypeExceptionTCP_IP type) throw()
{
    this->type = type;
}

TCP_IP::CustomException::~CustomException() throw()
{
}

// const char*	CustomException::what() const throw()
// {
//     eExceptType   te = this->exceptType;
//
//     if (te == eExceptType::e_ok)
//         return "ok";
//     if (te == eExceptType::e_need_server_work_mod)
//         return "e_need_server_work_mod";
//     if (te == eExceptType::e_need_client_work_mod)
//         return "e_need_client_work_mod";
//     if (te == eExceptType::e_need_any_work_mod)
//         return "e_need_any_work_mod";
//     if (te == eExceptType::e_WAN_wasChange)
//         return "e_WAN_wasChange";
//     return "Some error";
// }

TCP_IP::CustomException	&TCP_IP::CustomException::operator=(CustomException const & ref) throw ()
{
	this->type = ref.type;
	return *this;
}
