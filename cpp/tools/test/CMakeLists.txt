cmake_minimum_required(VERSION 3.5.1)

set(TESTS design_pattern_test.cc raft_test.cc)

add_executable(test test.cc ${TESTS})
include_directories(../include ../src/raft/)
find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(test gtest gmock glog Boost::serialization raft_grpc_proto)
