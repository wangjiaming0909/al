cmake_minimum_required(VERSION 2.8)

project(recipes)

# for recipes main
aux_source_directory(src SRC_DIR)
aux_source_directory(src/bytebuf SRC_DIR)
aux_source_directory(src/buffer SRC_DIR)
aux_source_directory(src/tests SRC_DIR)
aux_source_directory(src/guid_map SRC_DIR)
aux_source_directory(src/design_patterns SRC_DIR)
#aux_source_directory(src/easylogging SRC_DIR)
aux_source_directory(src/http_parser SRC_DIR)
aux_source_directory(src/http SRC_DIR)
aux_source_directory(src/string_piece SRC_DIR)
aux_source_directory(src/substring_search SRC_DIR)
aux_source_directory(src/utils SRC_DIR)
aux_source_directory(src/gperf SRC_DIR)
aux_source_directory(src/al SRC_DIR)



# for test main
aux_source_directory(test TEST_DIR)
aux_source_directory(src/gperf TEST_DIR)
aux_source_directory(test/gperf TEST_DIR)
aux_source_directory(src/buffer TEST_DIR)
aux_source_directory(test/buffer TEST_DIR)
aux_source_directory(src/substring_search TEST_DIR)
aux_source_directory(test/substring_search TEST_DIR)
aux_source_directory(src/string_piece TEST_DIR)
aux_source_directory(test/string_piece TEST_DIR)
aux_source_directory(src/http_parser TEST_DIR)
aux_source_directory(test/http_parser TEST_DIR)
aux_source_directory(src/http TEST_DIR)
aux_source_directory(test/http TEST_DIR)



add_executable(${PROJECT_NAME} ${SRC_DIR})
add_executable("gtests" ${TEST_DIR})
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17 -ggdb -O0 -Wfatal-errors " )
SET(CMAKE_CXX_COMPILER g++)

#find_package(Threads)
#target_link_libraries(recipes ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(recipes /boost_1_70_0/stage/lib/libboost_timer.a)
target_link_libraries(recipes /boost_1_70_0/stage/lib/libboost_chrono.a)
target_link_libraries(recipes /usr/local/lib/libfolly.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libevent.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libglog.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libdouble-conversion.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libgflags.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libiberty.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/liblz4.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/liblzma.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libsnappy.a)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libjemalloc.a)
target_link_libraries(recipes dl)
target_link_libraries(recipes pthread)
target_link_libraries(recipes unwind)
target_link_libraries(recipes /usr/lib/x86_64-linux-gnu/libc.a)


#target_link_libraries(gtests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gtests /usr/local/lib/libgtest.a)
target_link_libraries(gtests /usr/local/lib/libgmock.a)
target_link_libraries(gtests /boost_1_70_0/stage/lib/libboost_timer.a)
target_link_libraries(gtests /boost_1_70_0/stage/lib/libboost_chrono.a)
target_link_libraries(gtests /usr/local/lib/libfolly.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libevent.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libglog.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libdouble-conversion.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libgflags.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libiberty.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/liblz4.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/liblzma.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libsnappy.a)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libjemalloc.a)
target_link_libraries(gtests dl)
target_link_libraries(gtests pthread)
target_link_libraries(gtests unwind)
target_link_libraries(gtests /usr/lib/x86_64-linux-gnu/libc.a)
  

include_directories("src")
#include_directories("C:\\Users\\Jiaming\\Desktop\\boost_1_69_0")
include_directories("/boost_1_70_0")
ADD_DEFINITIONS(-DTESTING)
ADD_DEFINITIONS(-DELPP_THREAD_SAFE)
# for undefined reference to `_Ux86_64_init_local
ADD_DEFINITIONS(-DUNW_LOCAL_ONLY)
#ADD_DEFINITIONS(-DELPP_NO_DEFAULT_LOG_FILE)




