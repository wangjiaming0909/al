cmake_minimum_required(VERSION 3.5.1)


get_filename_component(raft_proto "../../protos/raft.proto" ABSOLUTE)
get_filename_component(raft_proto_path "${raft_proto}" PATH)

# Generated sources
set(raft_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/raft.pb.cc")
set(raft_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/raft.pb.h")
set(raft_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/raft.grpc.pb.cc")
set(raft_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/raft.grpc.pb.h")
add_custom_command(
  OUTPUT "${raft_proto_srcs}" "${raft_proto_hdrs}" "${raft_grpc_srcs}" "${raft_grpc_hdrs}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
  --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
  -I "${raft_proto_path}"
  --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
  "${raft_proto}"
  DEPENDS "${raft_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(HDRS raft_instance.h)
set(SOURCES raft_instance.cc)

add_library(raft
  ${raft_proto_srcs}
  ${raft_proto_hdrs}
  ${raft_grpc_srcs}
  ${raft_grpc_hdrs}
  ${HDRS}
  ${SOURCES})

target_link_libraries(raft
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
