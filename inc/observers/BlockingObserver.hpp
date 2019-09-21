#ifndef BLOCKING_OBSERVER_HPP
# define BLOCKING_OBSERVER_HPP

# include "lib.h"
# include "IObserver.hpp"
# include "Loger.hpp"

class BlockingObserver : public IObserver {
	BlockingObserver();
public:
	~BlockingObserver();
	BlockingObserver(BlockingObserver const & ref) = delete;
	BlockingObserver &operator=(BlockingObserver const & ref) = delete;
	static BlockingObserver	&getInstance();

	void 	operator()() override;
private:
	Loger 	_loger;
	void	_execute_list_request() override;
};

#endif
