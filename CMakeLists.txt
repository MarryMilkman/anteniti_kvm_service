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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -Wall -Wextra -Werror -g")


set(SRC
		${SRC_DIRECTORY}/Client.cpp

		${SRC_DIRECTORY}/controllers/CloudController.cpp
		${SRC_DIRECTORY}/Parser.cpp
		${SRC_DIRECTORY}/main.cpp
		${SRC_DIRECTORY}/ScriptExecutor.cpp
	)


add_executable(server_client ${SRC})
