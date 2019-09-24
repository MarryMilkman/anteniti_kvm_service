#ifndef TASK_HPP
# define TASK_HPP

# include "lib.h"

class TCP_IP;

#define TASK_FAIL_BROKEN_TCP_IP "task failed: broken tcp_ip connection"

enum 	eTaskStatus {
	ts_Created = 0,
	ts_InProgres,
	ts_Finish,
	ts_Used
};

class Task {
public:
	Task(std::string title, std::shared_ptr<TCP_IP> tcp_ip, std::string message);
	Task() = delete;
	~Task();
	Task(Task const & ref);
	Task &operator=(Task const & ref);

	void 	operator()();

	std::string 			title;
	std::string 			message;
	std::shared_ptr<TCP_IP>	tcp_ip;
	eTaskStatus 			status;
	std::string 			answer_message;
private:
};

#endif
