cmake_minimum_required(VERSION 3.5.1)

set(TESTS raft_test.cc reactor_test.cc)

add_executable(test test.cc ${TESTS})
include_directories(../include ../src)
find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(test gtest gmock glog Boost::serialization raft_grpc_proto raft)
