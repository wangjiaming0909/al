cmake_minimum_required(VERSION 3.5.1)


set(HDRS paxos_instance.h)
set(SOURCES paxos_instance.cc)

add_library(paxos
  ${HDRS}
  ${SOURCES})

target_link_libraries(paxos
  paxos_grpc_proto
  glog
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
