cmake_minimum_required(VERSION 3.26)
project(tool C CXX)
include(./cmake/common.cmake)
include(./cmake/proto.cmake)




add_subdirectory(deps/glog)
add_subdirectory(deps/googletest)
add_subdirectory(deps/libevent)
add_subdirectory(test)

add_subdirectory(src/helloworld)
add_subdirectory(src/raft)
add_subdirectory(src/paxos)
add_subdirectory(src/reactor)
