#!/bin/bash

if [ ! -e build ]; then
    mkdir build
fi

cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DMyOpenCVProject_BUILD_EXECUTABLE=1 #-D_CMAKE_TOOLCHAIN_PREFIX=llvm-
cmake --build . --target install
