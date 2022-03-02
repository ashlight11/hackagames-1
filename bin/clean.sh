#!/bin/bash

# at root directory:
rm *.log

#core:
if [ -d "build" ];then
rm -fr build
fi

if [ -d "hackagames/__pycache__" ];then
rm -fr hackagames/__pycache__
fi

#risky:
cd games/risky

if [ -d "build" ];then
rm -fr build
fi
if [ -d "__pycache__" ];then
rm -fr __pycache__
fi

#421:
cd ../421

if [ -d "__pycache__" ];then
rm -fr __pycache__
fi
