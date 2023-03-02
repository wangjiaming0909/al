cmake_minimum_required(VERSION 3.5.1)

add_executable(test test.cc design_pattern_test.cc)
include_directories(../include)
target_link_libraries(test gtest gmock glog)
