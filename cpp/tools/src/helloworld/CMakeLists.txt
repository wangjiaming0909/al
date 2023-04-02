cmake_minimum_required(VERSION 3.26.1)
project(hw)

# Targets greeter_[async_](client|server)
# Include generated *.pb.h files
foreach(_target
  greeter_client greeter_server 
  greeter_callback_client greeter_callback_server 
  greeter_async_client greeter_async_client2 greeter_async_server)
  add_executable(${_target} "${_target}.cc")
  target_link_libraries(${_target}
    helloworld_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()


