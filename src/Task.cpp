#include "Task.hpp"
#include "Tunnel.hpp"

Task::Task(std::string title, std::shared_ptr<Tunnel> tunnel, std::string message, int timeout) :
	tunnel(tunnel)
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
// 	tunnel(0)
// {
// 	this->status = eTaskStatus::ts_Created;
// }

Task::~Task() {
	// std::stringstream 	ss_error_log;
	//
	// ss_error_log << "remove tesk: " << this << "\n";
	// std::cerr << ss_error_log.str();
	// this->tunnel = 0;
}

Task::Task(Task const & ref) :
	tunnel(ref.tunnel)
{
	*this = ref;
}

Task &Task::operator=(Task const & ref) {
	this->title = ref.title;
	this->message = ref.message;
	this->tunnel = ref.tunnel;
	this->status = ref.status;
	this->timeout = ref.timeout;
	this->answer_message = ref.answer_message;
	return *this;
}


void 		Task::operator()() {
	std::cerr << "TASK START!\n";
	this->status = ts_InProgres;
	try {
		if (!this->tunnel)
			throw std::exception();
		std::lock_guard<std::mutex>	lock(this->tunnel->s_mutex);
		if (!this->tunnel->is_available)
			throw std::exception();
		// std::cerr << "start do task, memory addr tunnel:" << this->tunnel << ", title:" << title << "\n";

		std::cerr << "tcp was find\n";
		// std::cerr << this->tunnel << " ==8\n";
		this->tunnel->t_write(message);
		// std::cerr << this->tunnel << " ==8\n";
		this->answer_message = this->tunnel->t_read(this->timeout);
		if (!this->answer_message.size()) {
			throw std::exception();
		}
		// std::cerr << this->answer_message << "*******************\n";
	} catch (std::exception &e) {
		if (this->tunnel) {
			// this->tunnel->custom_disconnect();
			this->tunnel->is_available = false;
		}
		// this->tunnel = 0;
		std::cerr << "TASK_FAIL_BROKEN_TCP_IP\n";
		this->answer_message = TASK_FAIL_BROKEN_TCP_IP;
	}
	this->status = ts_Finish;
	std::cerr << "task finish\n";
}
