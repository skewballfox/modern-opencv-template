#!/bin/bash

if [ ! -e build ]; then
    mkdir build
fi

cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DMyOpenCVProject_BUILD_EXECUTABLE=1
cmake --build . --target install
