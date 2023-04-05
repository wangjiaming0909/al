cmake_minimum_required(VERSION 3.5.1)


set(HDRS raft_instance.h)
set(SOURCES raft_instance.cc)

add_library(raft
  ${HDRS}
  ${SOURCES})

target_link_libraries(raft
  raft_grpc_proto
  glog
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
