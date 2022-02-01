#!/bin/bash

# build
if [ ! -d "build" ];then
mkdir build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Outputs
cp libhackagames.so ..
cp libhackagames-interface.so ..
cp draft ..
# cp navgraph ../games/start-navgraph
# cp risky-easy ../games
