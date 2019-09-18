#ifndef MY_SQL_CONTROLLER
# define MY_SQL_CONTROLLER

# include "lib.h"

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

	std::string 				get_meshes_info_by_imei(std::string imei);
	std::vector<MySQLDataSegment *>	get_request(eRequestType type_request);
	bool 						story(MySQLDataSegment *data_segment);

private:
	std::mutex 					_mutex;

	std::vector<MySQLDataSegment *>	_get_request_info();
	std::vector<MySQLDataSegment *>	_get_request_block();
	std::vector<MySQLDataSegment *>	_get_request_setting();
};

#endif
