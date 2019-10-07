#ifndef TASK_HPP
# define TASK_HPP

# include "lib.h"

class Tunnel;

#define TASK_FAIL_BROKEN_TCP_IP "task_failed:_broken_tcp_ip_connection"

enum 	eTaskStatus {
	ts_Created = 0,
	ts_InProgres,
	ts_Finish,
	ts_Used
};

class Task {
public:
	Task(std::string title, std::shared_ptr<Tunnel> tunnel, std::string message, int timeout);
	Task() = delete;
	~Task();
	Task(Task const & ref);
	Task &operator=(Task const & ref);

	void 	operator()();

	std::string 			title;
	std::string 			message;
	std::shared_ptr<Tunnel>	tunnel;
	eTaskStatus 			status;
	std::string 			answer_message;
	int 					timeout;
private:
};

#endif
