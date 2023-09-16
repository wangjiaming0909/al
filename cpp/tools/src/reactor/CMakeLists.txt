cmake_minimum_required(VERSION 3.5.1)


set(HDRS reactor.h)
set(SOURCES reactor.cc)

add_library(reactor
  ${HDRS}
  ${SOURCES})

include_directories(${PROJECT_BINARY_DIR}/deps/libevent/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/libevent/include)

target_link_libraries(reactor event glog)
