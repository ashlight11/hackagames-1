#!/bin/bash

# build
if [ ! -d "dpd" ];then
mkdir dpd
fi

if [ ! -d "dpd/include" ];then
mkdir dpd/include
fi

# Get from github
git clone https://github.com/raysan5/raylib.git raylib
cd raylib
git checkout '3.0.0'

# let focus...
rm -fr games/*
touch games/CMakeLists.txt

# Then build...
mkdir build
cd build
cmake -DSHARED=ON -DSTATIC=ON ..
make

# and install localy...
cd src
cp libraylib.* ../../../dpd
cp *.h ../../../dpd/include
