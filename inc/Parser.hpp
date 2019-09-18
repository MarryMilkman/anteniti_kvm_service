#ifndef PARSER
#define PARSER

#include "lib.h"

class Mesh;

class Parser {
public:
    Parser();
    ~Parser();

	static std::vector<std::string> custom_split(std::string str, std::string delim);

	class SSHTunnel {
	public:
		static std::string	get_serial_number_from_authorization(std::string data_from_tunnel);
	};

	class MySQL {
	public:
		static std::vector<Mesh>	meshes_info_to_list_mesh(std::string info);
	};

};

#endif
