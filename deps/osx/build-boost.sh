#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

cd src/boost
./b2 toolset=clang address-model=64 link=static --layout=tagged \
cflags=-mmacosx-version-min=10.7 \
cxxflags="-std=c++11 -stdlib=libc++" \
linkflags="-std=c++11 -stdlib=libc++" \
macosx-version=10.11 \
macosx-version-min=10.7 \
install


#remove shared boost libs (keep only static libs)
#rm /usr/local/lib/libboost_*.dylib
