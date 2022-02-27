#!/bin/bash

# Update include files in dpd:
if [ ! -d "dpd" ];then
mkdir dpd
fi
cd dpd
if [ ! -d "dpd" ];then
mkdir include
fi
cp ../core-include/* ./include
cd ..

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

# Games
cp hg-risky ../games/risky
