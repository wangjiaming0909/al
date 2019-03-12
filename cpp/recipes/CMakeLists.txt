cmake_minimum_required(VERSION 2.8)

project(recipes)

aux_source_directory(src SRC_DIR)
aux_source_directory(src/bytebuf SRC_DIR)
add_executable(${PROJECT_NAME} ${SRC_DIR})
set( CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17 -ggdb -O0 -Wfatal-errors " )
include_directories("src")
include_directories("C:\\Users\\Jiaming\\Desktop\\boost_1_69_0")



