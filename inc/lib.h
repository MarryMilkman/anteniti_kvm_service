#ifndef LIB_H
#define LIB_H


/*
	function with exception:
MeshController::get_mesh_by (both)
Parser::SSHTunnel::get_serial_number_from_authorization
TCP_IP::custom_connect
TCP_IP::custom_read
TCP_IP::custom_write
TCP_IP::_init
Tunnel::t_refresh_connection
Tunnel::t_read
Tunnel::t_write

*/


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
#include <fcntl.h>

//#include <curl/curl.h>

// std::vector<std::string>    custom_split(std::string str, std::string delim);


enum eRequestType {
	rt_InfoRequest = 0,
	rt_BlockRequest,
	rt_SettingRequest,
	// rt_NewMeshRequest,
	// rt_NewRoutersRequest
};

#define INCORRECT_ADDRESSEE "incorrect_addressee"

#define PATH_VARIABLE_SETTING "/tmp/new_setting"

// define for comunicate:
#define CAN_NOT_FIND_MESH "can_not_find_mesh"

#define BLOCKLIST_CHANGE "block_list_changed"

#define SERVER_MOD_LOCK "server_mod_lock"
#define SERVER_MOD_FREE "server_mod_free"

#define SEND_MAC "send_mac"
#define SEND_INFO "send_info"
#define SEND_SETTING_VERSION "send_setting_version"

# define SETTING_CHANGED "setting_changed"

#define SETTING_DELIVERED "setting_delivered"
#define SETTING_NOT_DELIVERED "setting_not_delivered"

#define SETTING_APPLY "apply_setting"

#define SETTING_APPLYED "setting_applyed"
#define SETTING_NOT_APPLYED "setting_not_applyed"

#define SETTING_ROOL_BACK "setting_rool_back"


#define WAN_CHENGED "60"
#define WAN_DETECT "61"

#define ROUNER_NOT_AVAILABLE "router_is_not_available"

#define KEY_WAS_CHANGED "key_was_changed"
#define KEY_WAS_RESEIVED "key_was_delivered"

#define SETTING_SAVE "setting_save"

#endif
