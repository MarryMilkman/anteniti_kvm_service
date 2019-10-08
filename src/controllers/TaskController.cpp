#include "controllers/TaskController.hpp"
#include "Tunnel.hpp"


TaskController::TaskController() {

}

TaskController::~TaskController() {

}

TaskController	&TaskController::getInstance() {
	static TaskController _task_controller;

	return _task_controller;
}

	// make_new_task - get task from pool of task (_pull_task[MAX_TASK])
	//	if no free task in pool -> return 0
	//	else make new thread, and story tham to Task.thread_, then return Task
std::shared_ptr<Task>	TaskController::make_new_task(std::string title, std::shared_ptr<Tunnel> tunnel, std::string message, int timeout) {
	int 		i = 0;

	// this->_refresh_pull();
	while (i < MAX_TASK) {
		if (!this->_pull_task[i].task_ptr || this->_pull_task[i].task_ptr->status == eTaskStatus::ts_Used) {
			std::unique_lock<std::mutex> 	un_lock(this->_pull_mutex[i], std::try_to_lock);

			if (!un_lock.owns_lock()) {
				i++;
				continue;
			}
			this->_pull_task[i] = TaskController::CustomThread();
			this->_pull_task[i].task_ptr = std::shared_ptr<Task>(new Task(title, tunnel, message, timeout));
			this->_pull_task[i].thread_ = std::thread(std::ref(*this->_pull_task[i].task_ptr.get()));
			this->_pull_task[i].thread_.detach();
			std::cerr << "TsakController return " << i << " thread from pull\n";
			return this->_pull_task[i].task_ptr;
		}
		i++;
	}
	return 0;
}


// MARK : - class CustomThread

TaskController::CustomThread::CustomThread() {
	this->task_ptr = 0;
}

TaskController::CustomThread::~CustomThread() {
	if (this->thread_.joinable())
		this->thread_.join();
	this->task_ptr = 0;
}

TaskController::CustomThread::CustomThread(CustomThread const &ref) {
	*this = ref;
}

TaskController::CustomThread	&TaskController::CustomThread::operator=(CustomThread const &ref) {
	// this->thread_ = std::move(ref.thread_);
	this->task_ptr = ref.task_ptr;
	return *this;
}
