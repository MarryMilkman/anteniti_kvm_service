#ifndef TASK_CONTROLLER
# define TASK_CONTROLLER

# include "lib.h"

#include "Task.hpp"

#define MAX_TASK 60

class TaskController {

	struct CustomThread {
		std::shared_ptr<Task>	task_ptr;
		std::thread				thread_;

		CustomThread();
		CustomThread(CustomThread const &ref);
		CustomThread &operator=(CustomThread const &ref);
		~CustomThread();
	};

	TaskController();
public:
	~TaskController();
	TaskController(TaskController const & ref) = delete;
	TaskController operator=(TaskController const & ref) = delete;
	static TaskController	&getInstance();

	std::shared_ptr<Task>	make_new_task(std::string title, std::shared_ptr<TCP_IP> &tcp_ip, std::string message);
private:
	CustomThread	_pull_task[MAX_TASK];


};


#endif
