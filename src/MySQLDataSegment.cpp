#include "MySQLDataSegment.hpp"

// MARK : - class MySQLDataSegment

MySQLDataSegment::MySQLDataSegment() {
	this->id = 0;
	this->status = 0;
}

MySQLDataSegment::~MySQLDataSegment() {

}

MySQLDataSegment::MySQLDataSegment(MySQLDataSegment const &ref) {
	*this = ref;
}

MySQLDataSegment 	&MySQLDataSegment::operator=(MySQLDataSegment const &ref) {
	// this->list_sn.clear();
	// for (std::string sn : ref.list_sn)
	// 	this->list_sn.push_back(sn);

	// this->data = ref.data;
	this->imei = ref.imei;
	this->name_mesh = ref.name_mesh;
	this->status = ref.status;
	this->id = ref.id;
	this->answer_message = ref.answer_message;
	return *this;
}
