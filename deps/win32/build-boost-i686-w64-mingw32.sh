#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

cd src/boost

sudo ./b2 toolset=gcc address-model=32 target-os=windows threading=multi threadapi=win32 \
    cxxflags="-fvisibility=hidden -std=gnu++11" \
    link=static runtime-link=static --prefix=/usr/i686-w64-mingw32 --user-config=user-config.jam \
    --without-mpi --without-python -sNO_BZIP2=1 --layout=tagged variant=release install
