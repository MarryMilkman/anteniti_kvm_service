/*
	SettingObserver
class to track about new setting-request
*/




#include "controllers/MeshController.hpp"
#include "controllers/TaskController.hpp"
#include "observers/SettingObserver.hpp"
#include "MySQLDataSegment.hpp"
#include "Request.hpp"
#include "Mesh.hpp"

SettingObserver::SettingObserver() :
	_loger("../logs/setting_observer.logs")
{
}

SettingObserver::~SettingObserver() {
}

SettingObserver	&SettingObserver::getInstance() {
	static SettingObserver info_obs;

	return info_obs;
}

void 	SettingObserver::operator()() {
	while (1) {
		this->_execute_list_request(SETTING_CHANGED, eRequestType::rt_SettingRequest, 10);
		this->_check_untreated_list_request();
	}
}
