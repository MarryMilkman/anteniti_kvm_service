#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "lib.h"

class MySQLController;
class MySQLDataSegment;
class Task;


class Request {
public:
	Request() = delete;

	Request(MySQLDataSegment *mysql_data);
	~Request();
	Request(Request const &ref);
	Request	&operator=(Request const &ref);

	MySQLDataSegment 		*mysql_data;
	std::shared_ptr<Task>	task_ptr;
	int 					number_check;

	// RequestForInformation {
	// 	this->task = 0;
	// }
	// ~RequestForInformation {
	// 	if (this->task)
	// 		delete task;
	// 	this->task = 0;
	// }
};

#endif
