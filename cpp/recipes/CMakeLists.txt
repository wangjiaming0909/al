cmake_minimum_required(VERSION 2.8)

project(recipes)

aux_source_directory(src SRC_DIR)
aux_source_directory(src/bytebuf SRC_DIR)
aux_source_directory(src/buffer SRC_DIR)
aux_source_directory(src/tests SRC_DIR)
aux_source_directory(src/guid_map SRC_DIR)
aux_source_directory(src/design_patterns SRC_DIR)
aux_source_directory(src/easylogging SRC_DIR)

add_executable(${PROJECT_NAME} ${SRC_DIR})
set( CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17 -ggdb -O0 -Wfatal-errors " )

find_package(Threads)
target_link_libraries(recipes ${CMAKE_THREAD_LIBS_INIT})


include_directories("src")
include_directories("C:\\Users\\Jiaming\\Desktop\\boost_1_69_0")
ADD_DEFINITIONS(-DTESTING)
ADD_DEFINITIONS(-DELPP_THREAD_SAFE)
ADD_DEFINITIONS(-DELPP_NO_DEFAULT_LOG_FILE)




