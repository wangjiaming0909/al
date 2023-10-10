cmake_minimum_required(VERSION 3.5.1)


set(HDRS reactor.h reactor_impl.h event_reactor_impl.h timer.h uv_reactor_impl.h)
set(SOURCES reactor.cc reactor_impl.cc event_reactor_impl.cc timer.cc uv_reactor_impl.cc)

add_library(reactor
  ${HDRS}
  ${SOURCES})

include_directories(${PROJECT_BINARY_DIR}/deps/libevent/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/libevent/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/libuv/include)

target_link_libraries(reactor event glog event_pthreads uv)
