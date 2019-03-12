cmake_minimum_required(VERSION 2.8)

project(recipes)

aux_source_directory(src SRC_DIR)
aux_source_directory(src/bytebuf SRC_DIR)
add_executable(${PROJECT_NAME} ${SRC_DIR})
set( CMAKE_CXX_FLAGS "-std=c++17" )



