cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(multi_os_build)

IF(${WIN32})
	add_custom_target(multi_os_build COMMAND make -C ${mytest_SOURCE_DIR})
ELSEIF(${APPLE})
	add_custom_target(multi_os_build cd ${CMAKE_SOURCE_DIR} && bash make.sh )
ELSEIF(${UNIX})
	add_custom_target(mutli_os_build cd ${CMAKE_SOURCE_DIR} && bash make.sh /dev/ttyUSB0)
ENDIF()

