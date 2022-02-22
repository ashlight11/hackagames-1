#!/bin/bash

# Update include files in dpd:
cp ../core-include/* ../dpd/include

# build
if [ ! -d "build" ];then
mkdir build
fi

cd build
cmake -DTEST=ON -DGAMES=ON -DCMAKE_BUILD_TYPE=Debug ..
make

# Copy librairies:
cp libhackagames.so ../dpd
cp libhackagames-interface.so ../dpd

# Risky
cp hg-risky ../games/risky
