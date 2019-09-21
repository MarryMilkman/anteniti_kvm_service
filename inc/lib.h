#ifndef LIB_H
#define LIB_H

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdarg>
#include <sys/select.h>
#include <mutex>
#include <thread>
#include <chrono>

//#include <curl/curl.h>

// std::vector<std::string>    custom_split(std::string str, std::string delim);


enum eRequestType {
	rt_InfoRequest = 0,
	rt_BlockRequest,
	rt_SettingRequest,
	// rt_NewMeshRequest,
	// rt_NewRoutersRequest
};


#define PATH_VARIABLE_SETTING "/tmp/new_setting"

// define for comunicate:
#define BLOCK_DEVICE "Block_device"

#define SERVER_MOD_LOCK "Server_mod_lock"
#define SERVER_MOD_FREE "Server_mod_free"

#define SEND_INFO "Send_info"
#define SEND_SETTING_VERSION "Send_setting_version"

#define SETTING_CHENGED "Setting_chenged"

#define SETTING_DELIVERED "Setting_delivered"
#define SETTING_NOT_DELIVERED "Setting_not_delivered"

#define SETTING_APPLY "Apply_setting"

#define SETTING_APPLYED "Setting_applyed"
#define SETTING_NOT_APPLYED "Setting_not_applyed"

#define SETTING_ROOL_BACK "Setting_rool_back"


#define WAN_CHENGED "60"
#define WAN_DETECT "61"

#define ROUNER_NOT_AVAILABLE "Router_is_not_available"

#define KEY_WAS_CHANGED "Key_was_changed"
#define KEY_WAS_RESEIVED "Key_was_delivered"

#define SETTING_SAVE "Setting_save"

#endif
