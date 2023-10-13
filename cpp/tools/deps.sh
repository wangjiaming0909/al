### libevent
#mkdir -p deps
git clone https://github.com/libevent/libevent ./deps/libevent
### libuv
git clone https://github.com/libuv/libuv ./deps/libuv
### glog
git clone https://github.com/google/glog ./deps/glog

### googletest
git clone https://github.com/google/googletest ./deps/googletest
git clone --recurse-submodules -b v1.59.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc ./deps/grpc

