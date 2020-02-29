cmake_minimum_required(VERSION 2.8)

project(libevent)

aux_source_directory(src SRC_DIR)


add_executable(${PROJECT_NAME} ${SRC_DIR})
set(CMAKE_CXX_FLAGS "-O0 -g -Wall")
include_directories(${SRC_DIR})
include_directories("C:\\Users\\Jiaming\\Desktop\\boost_1_70_0\\boost_1_70_0")

