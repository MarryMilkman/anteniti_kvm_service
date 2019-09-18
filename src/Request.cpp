#include "Request.hpp"
#include "Task.hpp"
#include "controllers/MySQLController.hpp"
#include "MySQLDataSegment.hpp"

Request::~Request() {
	this->mysql_data = 0;
	if (this->task_ptr)
		this->task_ptr->status = eTaskStatus::ts_Used;
}

Request::Request(MySQLDataSegment *mysql_data_segment)
{
	this->mysql_data = mysql_data_segment;
	this->task_ptr = 0;
}

Request::Request(Request const &ref) {
	*this = ref;
}

Request 	&Request::operator=(Request const &ref) {
	this->mysql_data = ref.mysql_data;
	this->task_ptr = ref.task_ptr;
	this->number_check = ref.number_check;
	return *this;
}
