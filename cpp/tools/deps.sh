### libevent
#mkdir -p deps
git clone https://github.com/libevent/libevent ./deps/libevent
### glog
git clone https://github.com/google/glog ./deps/glog

### googletest
git clone https://github.com/google/googletest ./deps/googletest
git clone --recurse-submodules -b v1.52.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc ./deps/grpc
