#!/bin/bash



cd build

cmake -DGRPC_AS_SUBMODULE=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make -j12
