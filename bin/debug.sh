#!/bin/bash

# build
if [ ! -d "build" ];then
mkdir build
fi

cd build
cmake -DTEST=ON -DCMAKE_BUILD_TYPE=Debug ..
make

# Increase dependencies:
cp libhackagames.so ../dpd
cp libhackagames-interface.so ../dpd
cp core-include/* ../dpd/include

# Risky
cp hg-risky ../games/risky
