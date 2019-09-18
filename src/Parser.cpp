#include "Parser.hpp"
#include "Mesh.hpp"

Parser::Parser() {
}

Parser::~Parser() {
}


std::vector<std::string> Parser::custom_split(std::string str, std::string delim) {
	int 						i;
	std::vector<std::string>	r_list;
	std::string 				prom_str;


	while (str.size()) {
		i = str.find(delim);
		if (i < 0) {
			if (!str.empty())
			r_list.push_back(str);
			break;
		}
		str[i] = 0;
		r_list.push_back(str.c_str());
		str = (str.c_str() + i + delim.size());
	}
	return r_list;
}

std::string				Parser::SSHTunnel::get_serial_number_from_authorization(
											std::string data_from_tunnel)
{
	std::vector<std::string> data_segments = Parser::custom_split(data_from_tunnel, "***DELIM***");

	if (data_segments.size() != 2 || data_segments[0] != "intro")
		throw std::exception();
	return data_segments[1];
}

// need_realize

std::vector<Mesh> 	Parser::MySQL::meshes_info_to_list_mesh(std::string info) {
	info = "";
	return std::vector<Mesh>();
}
