#ifndef INFO_ORSERVER_HPP
# define INFO_ORSERVER_HPP

# include "lib.h"
# include "IObserver.hpp"
# include "Loger.hpp"

class InfoObserver : public IObserver {
	InfoObserver();
public:
	~InfoObserver();
	InfoObserver(InfoObserver const & ref) = delete;
	InfoObserver &operator=(InfoObserver const & ref) = delete;
	static InfoObserver 	&getInstance();

	void 	operator()() override;
private:
	Loger 	_loger;
	// void	_execute_list_request() override;
};

#endif
