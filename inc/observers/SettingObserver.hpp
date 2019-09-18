#ifndef SETTING_OBSERVER_HPP
# define SETTING_OBSERVER_HPP

# include "lib.h"
# include "IObserver.hpp"

class SettingObserver : public IObserver {
	SettingObserver();
public:
	~SettingObserver();
	SettingObserver(SettingObserver const & ref) = delete;
	SettingObserver &operator=(SettingObserver const & ref) = delete;
	static SettingObserver 	&getInstance();

	void 	operator()() override;
private:
	void	_execute_list_request() override;
};

#endif
