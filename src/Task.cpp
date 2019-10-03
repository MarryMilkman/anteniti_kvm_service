#include "Task.hpp"
#include "TCP_IP.hpp"

Task::Task(std::string title, std::shared_ptr<TCP_IP> tcp_ip, std::string message, int timeout) :
	tcp_ip(tcp_ip)
{
	this->title = title;
	this->message = message;
	this->status = eTaskStatus::ts_Created;
	this->timeout = timeout;
	// std::stringstream 	print_ss;
	//
	//  print_ss << "created task: " << this << "\n";
	//  std::cerr << print_ss.str();
}

// Task::Task() :
// 	tcp_ip(0)
// {
// 	this->status = eTaskStatus::ts_Created;
// }

Task::~Task() {
	// std::stringstream 	ss_error_log;
	//
	// ss_error_log << "remove tesk: " << this << "\n";
	// std::cerr << ss_error_log.str();
	// this->tcp_ip = 0;
}

Task::Task(Task const & ref) :
	tcp_ip(ref.tcp_ip)
{
	*this = ref;
}

Task &Task::operator=(Task const & ref) {
	this->title = ref.title;
	this->message = ref.message;
	this->tcp_ip = ref.tcp_ip;
	this->status = ref.status;
	this->timeout = ref.timeout;
	this->answer_message = ref.answer_message;
	return *this;
}


void 		Task::operator()() {
	std::cerr << "TASK START!\n";
	this->status = ts_InProgres;
	try {
		if (!this->tcp_ip)
			throw std::exception();
		std::lock_guard<std::mutex>	lock(this->tcp_ip->s_mutex);
		if (!this->tcp_ip->is_available)
			throw std::exception();
		// std::cerr << "start do task, memory addr tcp_ip:" << this->tcp_ip << ", title:" << title << "\n";

		std::cerr << "tcp was find\n";
		// std::cerr << this->tcp_ip << " ==8\n";
		this->tcp_ip->custom_write(message);
		// std::cerr << this->tcp_ip << " ==8\n";
		this->answer_message = this->tcp_ip->custom_read(this->timeout);
		if (!this->answer_message.size()) {
			throw std::exception();
		}
		// std::cerr << this->answer_message << "*******************\n";
	} catch (std::exception &e) {
		if (this->tcp_ip) {
			this->tcp_ip->custom_disconnect();
			this->tcp_ip->is_available = false;
		}
		// this->tcp_ip = 0;
		std::cerr << "TASK_FAIL_BROKEN_TCP_IP\n";
		this->answer_message = TASK_FAIL_BROKEN_TCP_IP;
	}
	this->status = ts_Finish;
	std::cerr << "task finish\n";
}
