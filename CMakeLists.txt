cmake_minimum_required(VERSION 3.5.1)

project(MyProject)

add_subdirectory(curl-7.65.3)

link_libraries(libcurl)

include_directories (
	./inc
)

set(SRC_DIRECTORY "./src")

SET(CMAKE_CXX_COMPILER g++-5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -Wall -Wextra -g")


set(SRC
		#${SRC_DIRECTORY}/Client.cpp
		${SRC_DIRECTORY}/Parser.cpp
		${SRC_DIRECTORY}/ScriptExecutor.cpp
		${SRC_DIRECTORY}/MySQLDataSegment.cpp
		${SRC_DIRECTORY}/TCP_IP.cpp
		${SRC_DIRECTORY}/Mesh.cpp
		${SRC_DIRECTORY}/Task.cpp
		${SRC_DIRECTORY}/Request.cpp

		${SRC_DIRECTORY}/controllers/MySQLController.cpp
		${SRC_DIRECTORY}/controllers/TaskController.cpp
		${SRC_DIRECTORY}/controllers/MeshConnectionController.cpp
		${SRC_DIRECTORY}/controllers/MeshController.cpp

		${SRC_DIRECTORY}/observers/IObserver.cpp
		${SRC_DIRECTORY}/observers/InfoObserver.cpp
		${SRC_DIRECTORY}/observers/SettingObserver.cpp
		${SRC_DIRECTORY}/observers/BlockingObserver.cpp

		${SRC_DIRECTORY}/main.cpp
	)


add_executable(kvm_service ${SRC})
