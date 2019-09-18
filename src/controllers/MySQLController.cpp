#include "controllers/MySQLController.hpp"
#include "MySQLDataSegment.hpp"

MySQLController::MySQLController() {

}

MySQLController::~MySQLController() {

}

MySQLController 	&MySQLController::getInstance() {
	static MySQLController 	_mysql_controller;

	return _mysql_controller;
}

std::vector<MySQLDataSegment *>	MySQLController::get_request(eRequestType type_request) {
	if (type_request == eRequestType::rd_InfoRequest)
		return this->_get_request_info();
	else if (type_request == eRequestType::rd_BlockRequest)
		return this->_get_request_block();
	else if (type_request == eRequestType::rd_SettingRequest)
		return this->_get_request_setting();
	else
		return std::vector<MySQLDataSegment *>();
}

bool 							MySQLController::story(MySQLDataSegment *data_segment) {
	if (!data_segment)
		return false;
	return true;
}



// MARK : - GETTERS

std::vector<MySQLDataSegment *>	MySQLController::_get_request_info() {
	return std::vector<MySQLDataSegment *>();
}

std::vector<MySQLDataSegment *>	MySQLController::_get_request_block() {
	return std::vector<MySQLDataSegment *>();
}

std::vector<MySQLDataSegment *>	MySQLController::_get_request_setting() {
	return std::vector<MySQLDataSegment *>();
}


// MARK : - get_meshes_info_by_imei
//	turn to tible with info about mesh (DEVICESN --_--)
std::string 	MySQLController::get_meshes_info_by_imei(std::string imei) {
	imei = "need_dealize";
	return "";
}
