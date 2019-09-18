#include "controllers/TaskController.hpp"

int 	TaskController::_max_task = 60;

TaskController::TaskController() {

}

TaskController::~TaskController() {

}

TaskController	&TaskController::getInstance() {
	static TaskController _task_controller;

	return _task_controller;
}

std::shared_ptr<Task>	TaskController::make_new_task(std::string title, TCP_IP *tcp_ip, std::string message) {
	int 		i;

	// this->_refresh_pull();
	while (i < TaskController::_max_task) {
		if (!this->_pull_task[i].task_ptr || this->_pull_task[i].task_ptr->status == eTaskStatus::ts_Used) {
			this->_pull_task[i] = TaskController::CustomThread();
			this->_pull_task[i].task_ptr = std::shared_ptr<Task>(new Task(title, tcp_ip, message));
			this->_pull_task[i].thread_ = std::thread(std::ref(*this->_pull_task[i].task_ptr));
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

TaskController::CustomThread &TaskController::CustomThread::operator=(CustomThread const &ref) {
	// this->thread_ = std::move(ref.thread_);
	this->task_ptr = ref.task_ptr;
	return *this;
}
