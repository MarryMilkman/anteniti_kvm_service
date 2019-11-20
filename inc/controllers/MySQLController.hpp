#ifndef MY_SQL_CONTROLLER
# define MY_SQL_CONTROLLER

# include "lib.h"
# include "Loger.hpp"

#define MYSQL_CONFIG "../mysql_config.txt"

class MySQLDataSegment;

enum eRequestStatus {
	rs_NotProcessed = 0,
	rs_InProgress,
	rs_Finish
};


class MySQLController {
	MySQLController();
public:
	~MySQLController();
	MySQLController(MySQLController const & ref) = delete;
	MySQLController operator=(MySQLController const & ref) = delete;
	static MySQLController	&getInstance();

	std::string 									get_meshes_info_by_imei(std::string imei);
	std::map<std::string, std::string> 				get_imei_and_name_by_serial_number(std::string serial_number);

	std::vector<std::shared_ptr<MySQLDataSegment>>	get_request(eRequestType type_request);
	bool 											story(std::shared_ptr<MySQLDataSegment> data_segment);

private:
	sql::Driver				*_driver;
	sql::Connection 		*_connector;
	sql::Statement			*_statement;

	std::string 			_url;
	std::string 			_user;
	std::string 			_pass;
	std::string 			_name_db;

	Loger 					_loger;
	std::mutex 				_mutex;
	std::mutex 				_mutex_for_execute;

	void 							_init_connection();
	bool							_update_all_list(eRequestType question);


	std::vector<std::shared_ptr<MySQLDataSegment>>	_get_request_info();
	std::vector<std::shared_ptr<MySQLDataSegment>>	_get_request_block();
	std::vector<std::shared_ptr<MySQLDataSegment>>	_get_request_setting();

	std::vector<std::shared_ptr<MySQLDataSegment>> _list_request_info;
	std::vector<std::shared_ptr<MySQLDataSegment>> _list_request_block;
	std::vector<std::shared_ptr<MySQLDataSegment>> _list_request_setting;
};

#endif
