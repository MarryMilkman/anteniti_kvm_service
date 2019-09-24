#include "Loger.hpp"

Loger::Loger(std::string name_file) :
	file(name_file)
{
	this->name_file = name_file;
}

Loger::~Loger() {
	this->file.close();
}

void 	Loger::make_log(std::string message, std::string n_file, int line) {
	// std::cerr << "MAKE LOG! " << n_file << ":" << line << "\n";
	if (!this->file.is_open()) {
		return;
	}
	// std::stringstream 	ss_heder;
	//
	// ss_heder << "File: " << n_file << ";\nLine: " << line << ";\n" << message << "\n";
	// this->file << (ss_heder.str());
}

void 	Loger::make_log(std::string message) {
	// std::cerr << "MAKE LOG!\n";

	if (!this->file.is_open()) {
		return;
	}
	// std::stringstream 	ss_heder;
	//
	// ss_heder << message << "\n";
	// this->file << ss_heder.str();
}
