#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "lib.h"

class MySQLController;
class MySQLDataSegment;
class Task;


class Request {
public:
	Request() = delete;

	Request(std::shared_ptr<MySQLDataSegment> mysql_data);
	~Request();
	Request(Request const &ref);
	Request	&operator=(Request const &ref);

	std::shared_ptr<MySQLDataSegment>	mysql_data;
	std::shared_ptr<Task>				task_ptr;
	int 								number_check;
	bool 								is_mesh_find;

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
