#ifndef LOGER_HPP
# define LOGER_HPP

# include "lib.h"

class Loger {
public:
	Loger(std::string name_file);
	~Loger();

	std::string 	name_file;
	std::ofstream	file;

	void 	make_log(std::string message, std::string n_file, int line);

	void 	make_log(std::string message);

private:
};

#endif
