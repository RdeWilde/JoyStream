#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

export BOOST_ROOT=$PWD/src/boost/
export PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent/
$PWD/../boost/b2 install toolset=gcc address-model=32 target-os=windows threading=multi threadapi=win32 \
        cxxflags="-fvisibility=hidden -std=gnu++11" \
        runtime-link=static link=static --user-config=user-config.jam \
        --without-python boost=system boost-link=static variant=release --prefix=$PREFIX_PATH \
        geoip=off deprecated-functions=off logging=none asserts=off
