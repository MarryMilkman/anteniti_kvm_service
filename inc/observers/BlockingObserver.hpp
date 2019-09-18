#ifndef BLOCKING_OBSERVER_HPP
# define BLOCKING_OBSERVER_HPP

# include "lib.h"
# include "IObserver.hpp"

class BlockingObserver : public IObserver {
	BlockingObserver();
public:
	~BlockingObserver();
	BlockingObserver(BlockingObserver const & ref) = delete;
	BlockingObserver &operator=(BlockingObserver const & ref) = delete;
	static BlockingObserver	&getInstance();

	void 	operator()() override;
private:
	void	_execute_list_request() override;
};

#endif
