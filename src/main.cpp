#include "lib.h"
#include "Client.hpp"


std::vector<std::string> custom_split(std::string str, std::string delim) {
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

int main() {
	while (1) {
		try {
			Client("0.0.0.0", 1);
		}
		catch (std::exception &e) {}
	}
	return 0;
}
