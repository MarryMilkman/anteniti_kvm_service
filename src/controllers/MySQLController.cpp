#include "controllers/MySQLController.hpp"
#include "MySQLDataSegment.hpp"

MySQLController::MySQLController() :
	_loger("../logs/mysql_controller.logs")
{
	this->_connector = 0;
	this->_statement = 0;
	this->_driver = get_driver_instance();
	this->_init_connection();
	std::cerr << this->_connector << "\n";
	std::cerr << this->_statement << "\n";
}

MySQLController::~MySQLController() {
	if (this->_statement) {
		// std::cerr << "delete _statement in MySQLController  MySQLController::_init_connection\n";
		delete this->_statement;
	}
	if (this->_connector) {
		// std::cerr << "delete _connector in MySQLController  MySQLController::_init_connection\n";
		delete this->_connector;
	}
	if (this->_driver) {
		this->_driver->threadEnd();
	}
}

MySQLController 	&MySQLController::getInstance() {
	static MySQLController 	_mysql_controller;

	return _mysql_controller;
}

// _init_connection
//	init and reinit (if needed) MySQL connection
void 	MySQLController::_init_connection() {
	if (this->_statement) {
		// std::cerr << "delete _statement in MySQLController  MySQLController::_init_connection\n";
		delete this->_statement;
	}
	if (this->_connector) {
		// std::cerr << "delete _connector in MySQLController  MySQLController::_init_connection\n";
		delete this->_connector;
	}
	this->_connector = 0;
	this->_statement = 0;
	try {
		std::string 	word;
		std::fstream 	file(MYSQL_CONFIG);

		while (file >> word) {
			if (word == "url")
				file >> this->_url;
			if (word == "user")
				file >> this->_user;
			if (word == "pass")
				file >> this->_pass;
			if (word == "name_db")
				file >> this->_name_db;
		}

		this->_connector = this->_driver->connect(this->_url, this->_user, this->_pass);
		this->_connector->setSchema(this->_name_db);
		this->_statement = this->_connector->createStatement();
	}
	catch (sql::SQLException &e) {
		std::cerr << e.what() << "\ncode error: " << e.getErrorCode() << "\n";
		if (e.getErrorCode() == 2006)
			this->_init_connection();
	}
}

// story
//	story std::shared_ptr<MySQLDataSegment> to myqsl data base
bool 							MySQLController::story(std::shared_ptr<MySQLDataSegment>data_segment) {
	std::stringstream 	ss_sql_requeset;

	std::cerr << "Story start....\n";
	if (!this->_connector || !this->_statement) {
		this->_init_connection();
		if (!data_segment || !this->_statement)
			return false;
	}
	ss_sql_requeset << "UPDATE KVM SET Status = \'2\', ANS_Message = \'"
					<< data_segment->answer_message << "\' WHERE ID = \'" << data_segment->id << "\'";
	// this->_remove_from_list_request(data_segment);
	// std::cerr << ss_sql_requeset.str() << "\n";
	if (data_segment) {
		// std::cerr << "delete data_segment in MySQLController  MySQLController::story\n";

		data_segment = 0;
	}
	{
		std::lock_guard<std::mutex> execut_lock(this->_mutex_for_execute);
		try {
			this->_statement->execute(ss_sql_requeset.str());
		} catch (sql::SQLException &e) {
			std::cerr << e.what() << "\ncode error: " << e.getErrorCode() << "\n";
			if (e.getErrorCode() == 2006)
				this->_init_connection();
		}
	}
	std::cerr << "Story proshel gladko....\n";
	return true;
}

// MARK : - get_request
	//	get type of need list_request; depending on type calls the metod, that return ask data
std::vector<std::shared_ptr<MySQLDataSegment>>	MySQLController::get_request(eRequestType type_request) {
	if (type_request == eRequestType::rt_InfoRequest)
		return this->_get_request_info();
	else if (type_request == eRequestType::rt_BlockRequest)
		return this->_get_request_block();
	else if (type_request == eRequestType::rt_SettingRequest)
		return this->_get_request_setting();
	else
		return std::vector<std::shared_ptr<MySQLDataSegment>>();
}


// MARK : - GETTERS requests

	// _get_request_info - looking through _list_request_info and check every MySQLDataSegment.status
	//		if status == 0 (NotProcessed) - story ro return list
	//		if status == 2 (Finish) - unstory data from _list_request_info
	//	if return_list.size > 0 -> return this list
	//	else start update ALL request_list (_update_all_list), and than try repit procedure
	//	and in any case (return_list.size) return r_list;
std::vector<std::shared_ptr<MySQLDataSegment>>	MySQLController::_get_request_info() {
	std::vector<std::shared_ptr<MySQLDataSegment>>	r_list;

	for (int i = 0, size = this->_list_request_info.size(); i < size;) {
		std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_info[i];
		if (ms_data->status == 0)
			r_list.push_back(ms_data);
		else if (ms_data->status == 2) {
			this->_list_request_info.erase(this->_list_request_info.begin() + i);
			size = this->_list_request_info.size();
			continue;
		}
		i++;
	}
	if (r_list.size())
		return r_list;
	std::unique_lock<std::mutex> lock(this->_mutex, std::try_to_lock);
	if (lock.owns_lock() && this->_update_all_list(eRequestType::rt_InfoRequest)) {
		for (int i = 0, size = this->_list_request_info.size(); i < size;) {
			std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_info[i];
			if (ms_data->status == 0)
				r_list.push_back(ms_data);
			else if (ms_data->status == 2) {
				this->_list_request_info.erase(this->_list_request_info.begin() + i);
				size = this->_list_request_info.size();
				continue;
			}
			i++;
		}
	}
	return r_list;
}

	// _get_request_block
	// same as _get_request_info, bat hear we loking for _list_request_block
std::vector<std::shared_ptr<MySQLDataSegment>>	MySQLController::_get_request_block() {
	std::vector<std::shared_ptr<MySQLDataSegment>>	r_list;

	for (int i = 0, size = this->_list_request_block.size(); i < size;) {
		std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_block[i];
		if (ms_data->status == 0)
			r_list.push_back(ms_data);
		else if (ms_data->status == 2) {
			this->_list_request_block.erase(this->_list_request_block.begin() + i);
			size = this->_list_request_block.size();
			continue;
		}
		i++;
	}
	if (r_list.size())
		return r_list;
	std::unique_lock<std::mutex> lock(this->_mutex, std::try_to_lock);
	if (lock.owns_lock() && this->_update_all_list(eRequestType::rt_BlockRequest)) {
		for (int i = 0, size = this->_list_request_block.size(); i < size;) {
			std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_block[i];

			if (ms_data->status == 0)
				r_list.push_back(ms_data);
			else if (ms_data->status == 2) {
				this->_list_request_block.erase(this->_list_request_block.begin() + i);
				size = this->_list_request_block.size();
				continue;
			}
			i++;
		}
	}
	return r_list;
}

	// _get_request_setting
	// same as _get_request_info, bat hear we loking for _list_request_setting
std::vector<std::shared_ptr<MySQLDataSegment>>	MySQLController::_get_request_setting() {
	std::vector<std::shared_ptr<MySQLDataSegment>>	r_list;

	for (int i = 0, size = this->_list_request_setting.size(); i < size;) {
		std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_setting[i];
		if (ms_data->status == 0)
			r_list.push_back(ms_data);
		else if (ms_data->status == 2) {
			this->_list_request_setting.erase(this->_list_request_setting.begin() + i);
			size = this->_list_request_setting.size();
			continue;
		}
		i++;
	}
	if (r_list.size())
		return r_list;
	std::unique_lock<std::mutex> lock(this->_mutex, std::try_to_lock);
	if (lock.owns_lock() && this->_update_all_list(eRequestType::rt_SettingRequest)) {
		for (int i = 0, size = this->_list_request_setting.size(); i < size;) {
			std::shared_ptr<MySQLDataSegment>	ms_data = this->_list_request_setting[i];
			if (ms_data->status == 0)
				r_list.push_back(ms_data);
			else if (ms_data->status == 2) {
				this->_list_request_setting.erase(this->_list_request_setting.begin() + i);
				size = this->_list_request_setting.size();
				continue;
			}
			i++;
		}
	}
	return r_list;
}


// update all request from db. if have some new request with type == question -> return true
bool			MySQLController::_update_all_list(eRequestType question) {
	bool 				q = false;
	int 				max_ip = -1;
	std::stringstream 	ss_sql_requeset;
	sql::ResultSet 		*result = 0;

	if (!this->_connector || !this->_statement) {
		this->_init_connection();
		if (!this->_connector || !this->_statement)
			return false;
	}
	ss_sql_requeset << "SELECT ID, Type, IMEI, NAME_MESH, Status FROM KVM WHERE Status = \'0\'";
	try {
		{
			std::lock_guard<std::mutex> execut_lock(this->_mutex_for_execute);
			result = this->_statement->executeQuery(ss_sql_requeset.str());
		}
		while (result && result->next()) {
			std::shared_ptr<MySQLDataSegment>	data_segment = std::shared_ptr<MySQLDataSegment>(new MySQLDataSegment);
			std::string 						s_type = result->getString("Type");

			try {
				data_segment->id = std::stoi(result->getString("ID"));
			} catch (std::exception &e) {
				data_segment->id = -1;
			}

			data_segment->imei = result->getString("IMEI");
			data_segment->name_mesh = result->getString("NAME_MESH");
			try {
				data_segment->status = std::stoi(result->getString("Status"));
			} catch (std::exception &e) {
				data_segment->status = -1;
			}
			if (data_segment->id > max_ip)
			max_ip = data_segment->id;
			if (s_type == "setting") {
				if (question == rt_SettingRequest)
				q = true;
				data_segment->type = rt_SettingRequest;
				this->_list_request_setting.push_back(data_segment);
			}
			else if (s_type == "info") {
				if (question == rt_InfoRequest)
				q = true;
				data_segment->type = rt_InfoRequest;
				this->_list_request_info.push_back(data_segment);
			}
			else if (s_type == "block") {
				if (question == rt_BlockRequest)
				q = true;
				data_segment->type = rt_BlockRequest;
				this->_list_request_block.push_back(data_segment);
			}
			else {
				this->_loger.make_log("delete data_segment\n", __FILE__, __LINE__);
				data_segment = 0;
			}
		}
		if (result) {
			delete result;
		}
		if (max_ip >= 0) {
			ss_sql_requeset.str(std::string());
			ss_sql_requeset << "UPDATE KVM SET Status = \'1\' WHERE ID < \'" << max_ip + 1 << "\' AND Status = \'0\'";
			std::cerr << ss_sql_requeset.str() << "\n";
			{
				std::lock_guard<std::mutex> execut_lock(this->_mutex_for_execute);
				this->_statement->execute(ss_sql_requeset.str());
			}
		}
	}
	catch (sql::SQLException &e) {
		std::cerr << e.what() << "\ncode error: " << e.getErrorCode() << "\n";
		if (e.getErrorCode() == 2006)
			this->_init_connection();
	}
	return q;
}



// MARK : - get_meshes_info_by_imei
//	accesses the table for mesh information (DEVICESN --_--)
std::string		MySQLController::get_meshes_info_by_imei(std::string imei) {
	std::stringstream 	ss_sql_requeset;
	std::string			r_str = "";
	sql::ResultSet 		*result = 0;

	if (!this->_connector || !this->_statement) {
		this->_init_connection();
		if (!this->_connector || !this->_statement)
			return r_str;
	}
	ss_sql_requeset << "SELECT SN, NETWORK FROM DEVICESN WHERE IMEI = \'" << imei <<"\'";
	try {
		// std::cerr << ss_sql_requeset.str() << "\n----------\n";
		{
			std::lock_guard<std::mutex> execut_lock(this->_mutex_for_execute);
			result = this->_statement->executeQuery(ss_sql_requeset.str());
		}
		while (result && result->next()) {
			r_str += imei + " " + result->getString("NETWORK") + " " + result->getString("SN") + "\n";
			// std::cerr << "result line: " << r_str << "\n";
		}
		if (result) {
			// std::cerr << "delete result in MySQLController MySQLController::get_meshes_info_by_imei\n";
			delete result;
		}
	}
	catch (sql::SQLException &e) {
		std::cerr << e.what() << "\ncode error: " << e.getErrorCode() << "\n";
		if (e.getErrorCode() == 2006)
			this->_init_connection();
	}
	return r_str;
}

std::map<std::string, std::string> 				MySQLController::get_imei_and_name_by_serial_number(std::string serial_number) {
	std::stringstream 					ss_sql_requeset;
	std::map<std::string, std::string>	r_map;
	sql::ResultSet 						*result = 0;

	if (!this->_connector || !this->_statement) {
		this->_init_connection();
		if (!this->_connector || !this->_statement)
			return r_map;
	}
	ss_sql_requeset << "SELECT IMEI, NETWORK FROM DEVICESN WHERE SN = \'" << serial_number <<"\'";
	try {
		// std::cerr << ss_sql_requeset.str() << "\n----------\n";
		{
			std::lock_guard<std::mutex> execut_lock(this->_mutex_for_execute);
			result = this->_statement->executeQuery(ss_sql_requeset.str());
		}
		while (result && result->next()) {
			r_map["imei"] = result->getString("IMEI");
			r_map["name_mesh"] = result->getString("NETWORK");
			// std::cerr << "result line: " << r_str << "\n";
		}
		if (result) {
			// std::cerr << "delete result in MySQLController MySQLController::get_meshes_info_by_imei\n";
			delete result;
		}
	}
	catch (sql::SQLException &e) {
		std::cerr << e.what() << "\ncode error: " << e.getErrorCode() << "\n";
		if (e.getErrorCode() == 2006)
			this->_init_connection();
	}
	return r_map;
}
