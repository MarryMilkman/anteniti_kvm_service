#ifndef MY_SQL_DATA_SEGMENT_HPP
#define MY_SQL_DATA_SEGMENT_HPP

#include "lib.h"

class MySQLDataSegment {
public:
	MySQLDataSegment();
	~MySQLDataSegment();
	MySQLDataSegment(MySQLDataSegment const &ref);
	MySQLDataSegment	&operator=(MySQLDataSegment const &ref);

	std::string 				imei;
	std::string 				name_mesh;
	// std::vector<std::string>	list_sn;
	// std::string 				data;

	int 						status;
	eRequestType 				type;
	// for story
	int 						id;
	std::string 				answer_message;
};

#endif
