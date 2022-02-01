#!/bin/bash

# big header
cat src/dependancy.h  > hackagames.h
echo "
#ifndef HACKAGAMES_H
#define HACKAGAMES_H" >> hackagames.h
cat src/geometry.h    >> hackagames.h
echo "
"                     >> hackagames.h
cat src/organism.h    >> hackagames.h
echo "
"                     >> hackagames.h
cat src/game.h        >> hackagames.h
echo "
#endif"               >> hackagames.h

# build
if [ ! -d "build" ];then
mkdir build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Outputs
cp libhackagame.so ..
cp draft ..
cp navgraph ../games/start-navgraph
cp risky-easy ../games
