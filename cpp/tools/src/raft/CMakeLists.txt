cmake_minimum_required(VERSION 3.5.1)


set(HDRS raft_instance.h)
set(SOURCES raft_instance.cc)

add_library(raft
  ${HDRS}
  ${SOURCES})

include_directories(${PROJECT_BINARY_DIR}/deps/libevent/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/libevent/include)

target_link_libraries(raft
  event
  raft_grpc_proto
  glog
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
