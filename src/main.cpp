#include "lib.h"
#include "controllers/MySQLController.hpp"
#include "controllers/MeshController.hpp"
#include "controllers/PortController.hpp"
#include "controllers/TunnelController.hpp"

#include "observers/BlockingObserver.hpp"
#include "observers/SettingObserver.hpp"
#include "observers/InfoObserver.hpp"

int main() {
	signal(SIGPIPE, SIG_IGN);

		// TunnelController
	TunnelController 	&_tunnel_controller = TunnelController::getInstance();
		// MySQLController init
	MySQLController::getInstance();
		//PortController init
	PortController::getInstance();
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
	//
	_tunnel_controller.monitor_tunnels();

	if (thread_info_observer.joinable())
		thread_info_observer.join();
	if (thread_setting_observer.joinable())
		thread_setting_observer.join();
	if (thread_bloching_observer.joinable())
		thread_bloching_observer.join();
	//

	return 0;
}
