/*
	BlockingObserver
class to track about new block-request

_list_untreated_request - list request, that alredy not processed (thay in progress)
*/


#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/BlockingObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

BlockingObserver::BlockingObserver() :
	_loger("../logs/blocking_observer.logs")
{
}

BlockingObserver::~BlockingObserver() {
}

BlockingObserver	&BlockingObserver::getInstance() {
	static BlockingObserver info_obs;

	return info_obs;
}

void 	BlockingObserver::operator()() {
	while (1) {
		this->_execute_list_request(BLOCKLIST_CHANGE, eRequestType::rt_BlockRequest, 30);
			// virtual method (override)
		this->_check_untreated_list_request();
	}
}
