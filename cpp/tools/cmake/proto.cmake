cmake_minimum_required(VERSION 3.26)

function(build_proto proto_file)
  #message("proto_file: " ${proto_file})
  get_filename_component(proto_full_dir ${proto_file} ABSOLUTE)
  #message("proto_full_dir: " ${proto_full_dir})
  get_filename_component(proto_file_name ${proto_file} NAME_WE)
  #message("proto_file_name: " ${proto_file_name})
  get_filename_component(proto_path "${proto_full_dir}" PATH)
  #message("proto_path: " ${proto_path})

  set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.pb.cc")
  set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.pb.h")
  set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.grpc.pb.cc")
  set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.grpc.pb.h")
  add_custom_command(
    OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
      --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
      -I "${proto_path}"
      -- plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
      "${proto_full_dir}"
    DEPENDS "${proto_full_dir}")

  #message("-------" ${proto_srcs} ${proto_hdrs})
  #message("-------" ${grpc_srcs} ${grpc_hdrs})

  return(PROPAGATE proto_srcs proto_hdrs grpc_srcs grpc_hdrs)
endfunction(build_proto)

## argv0-4: proto_file proto_srcs proto_hdrs grpc_srcs grpc_hdrs
macro(build_proto_macro)
  get_filename_component(proto_full_dir ${ARGV0} ABSOLUTE)
  get_filename_component(proto_file_name ${ARGV0} NAME_WE)
  get_filename_component(proto_path "${proto_full_dir}" PATH)

  set(${ARGV1}  "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.pb.cc")
  set(${ARGV2}  "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.pb.h")
  set(${ARGV3} "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.grpc.pb.cc")
  set(${ARGV4} "${CMAKE_CURRENT_BINARY_DIR}/${proto_file_name}.grpc.pb.h")
  message("----------- " ${ARGV0})
  message("----------- " ${ARGV1})
  message("----------- " ${CMAKE_CURRENT_BINARY_DIR})
  message("command: " ${_PROTOBUF_PROTOC})
  message("proto_full_dir: " ${proto_full_dir})
  message("proto_file_name: " ${proto_file_name})
  message("proto_path: " ${proto_path})
  add_custom_command(
    OUTPUT "${${ARGV1}}" "${${ARGV2}}" "${${ARGV3}}" "${${ARGV4}}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
      --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
      -I "${proto_path}"
      --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
      "${proto_full_dir}"
    DEPENDS "${proto_full_dir}")
endmacro(build_proto_macro)

## argv0 ./protos/helloworld.proto
## new library name is helloworld_grpc_proto
macro(build_proto_lib)
  set(proto_file ${ARGV0})

  get_filename_component(proto_name ${proto_file} NAME_WE)
  set(library_name "${proto_name}_grpc_proto")

  set(proto_srcs "")
  set(proto_hdrs "")
  set(grpc_srcs "")
  set(grpc_hdrs "")
  build_proto_macro(${proto_file} proto_srcs proto_hdrs grpc_srcs grpc_hdrs)

  # Include generated *.pb.h files
  include_directories("${CMAKE_CURRENT_BINARY_DIR}")
  include_directories("include")
  file(GLOB_RECURSE INCLUDE_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

  # hw_grpc_proto
  add_library(${library_name} ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs} ${INCLUDE_HDRS})
  target_link_libraries(${library_name} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
endmacro(build_proto_lib)

build_proto_lib("./protos/helloworld.proto")
build_proto_lib("./protos/raft.proto")
