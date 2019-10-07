#include "lib.h"
#include "controllers/MySQLController.hpp"
#include "controllers/MeshController.hpp"
#include "controllers/PortController.hpp"

#include "observers/BlockingObserver.hpp"
#include "observers/SettingObserver.hpp"
#include "observers/InfoObserver.hpp"

int main() {
	signal(SIGPIPE, SIG_IGN);

		// MySQLController init
	MySQLController::getInstance();
		//PortController init
	PortController		&_port_controller = PortController::getInstance();
		// MeshController init
	MeshController::getInstance();
		// BlockingObserver init
	BlockingObserver	&_bloching_observer = BlockingObserver::getInstance();
		// SettingObserver init
	SettingObserver 	&_setting_observer = SettingObserver::getInstance();
		// InfoObserver init
	InfoObserver		&_info_observer = InfoObserver::getInstance();

	std::thread 	thread_info_observer(std::ref(_info_observer));
	std::thread 	thread_bloching_observer(std::ref(_bloching_observer));
	std::thread 	thread_setting_observer(std::ref(_setting_observer));

	std::cerr << "start join....\n";

	_port_controller.watch_used_port();
	//
	if (thread_info_observer.joinable())
		thread_info_observer.join();
	if (thread_setting_observer.joinable())
		thread_setting_observer.join();
	if (thread_bloching_observer.joinable())
		thread_bloching_observer.join();
	//

	return 0;
}
