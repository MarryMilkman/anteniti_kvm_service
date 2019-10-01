#ifndef I_OBSERVER_HPP
# define I_OBSERVER_HPP

# include "lib.h"
# include "controllers/MySQLController.hpp"


class TaskController;
class Request;
class MeshController;

enum eRequestType;

class 	IObserver {
public:
	~IObserver();
	IObserver(IObserver const & ref) = delete;
	IObserver &operator=(IObserver const & ref) = delete;

	virtual void 						operator()() = 0;

protected:
	IObserver();

	MySQLController 					&_mysql_controller;
	TaskController						&_task_controller;
	MeshController 						&_mesh_controller;
	std::vector<Request>				_list_untreated_request;


	void 			_refresh_untreated_list_request(eRequestType type_request);
	void 			_check_untreated_list_request();
	void 			_execute_list_request(std::string message_for_task, eRequestType type_request);
};

#endif
