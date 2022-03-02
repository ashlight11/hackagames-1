#!/bin/bash

# build
if [ ! -d "421" ];then
mkdir 421
fi

# Update include files in dpd:
cp -r games/421/* 421
rm 421/dpd 421/hackagames
cp -r dpd hackagames LICENCE.md 421
zip -r hackagames-421.zip 421
rm -fr 421
