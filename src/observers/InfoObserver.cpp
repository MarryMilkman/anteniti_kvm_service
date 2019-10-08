/*
	InfoObserver
class to track about new info-request
*/




#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/InfoObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

InfoObserver::InfoObserver() :
	_loger("../logs/info_observer.logs")
{
}

InfoObserver::~InfoObserver() {
}

InfoObserver	&InfoObserver::getInstance() {
	static InfoObserver info_obs;

	return info_obs;
}

void 	InfoObserver::operator()() {
	while (1) {
		// std::cerr << "jojo\n";
		this->_execute_list_request(SEND_INFO, eRequestType::rt_InfoRequest, 2);
		this->_check_untreated_list_request();
	}
}
