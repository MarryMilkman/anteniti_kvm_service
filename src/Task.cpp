#include "Task.hpp"
#include "TCP_IP.hpp"

Task::Task(std::string title, TCP_IP *tcp_ip, std::string message) {
	this->title = title;
	this->tcp_ip = tcp_ip;
	this->message = message;
	this->status = eTaskStatus::ts_Created;
}

Task::Task() {
	this->tcp_ip = 0;
	this->status = eTaskStatus::ts_Created;
}

Task::~Task() {

}

Task::Task(Task const & ref) {
	*this = ref;
}

Task &Task::operator=(Task const & ref) {
	this->title = ref.title;
	this->message = ref.message;
	this->tcp_ip = ref.tcp_ip;
	this->status = ref.status;
	this->answer_message = ref.answer_message;
	return *this;
}


void 		Task::operator()() {
	std::cerr << "WI DO TASK!!!!!!!!!!!!!!!!!!!!\n";
	this->status = ts_InProgres;
	try {
		if (!this->tcp_ip)
			throw std::exception();
		std::cerr << "TCP_IP was FIND????????????????!!!!!!!!!!!!!!!!!!\n";
		this->tcp_ip->custom_write(message);
		// this->answer_message += this->tcp_ip->custom_read();
		// std::cerr << this->answer_message << "*******************\n";
	} catch (std::exception &e) {
		this->answer_message = TASK_FAIL_BROKEN_TCP_IP;
	}
	this->status = ts_Finish;
	std::cerr << "TAsk FINISH!\n";
}
