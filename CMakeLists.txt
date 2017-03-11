cmake_minimum_required (VERSION 2.6)
project (myonsh)

set (myonsh_VERSION_MAJOR 0)
set (myonsh_VERSION_MINOR 1)

#configure_file (
#	"${PROJECT_SOURCE_DIR}/myonsh_config.h.in"
#	"${PROJECT_BINARY_DIR}/myonsh_config.h"
#)

include_directories ("${PROJECT_BINARY_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(myonsh
	src/main.cpp
	src/application.cpp
)


