#!/bin/bash

# build
if [ ! -d "risky" ];then
mkdir risky
fi

# Update include files in dpd:
cp -r games/risky/* risky
rm risky/dpd
cp -r dpd risky/dpd
zip -r hackagames-risky.zip risky
rm -fr risky
