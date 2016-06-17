#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

export BOOST_ROOT=$PWD/src/boost/
PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent

$BOOST_ROOT/b2 install toolset=clang address-model=64 boost-link=static link=static deprecated-functions=off logging=off asserts=off \
cflags="-mmacosx-version-min=10.7" \
cxxflags="-std=c++11 -stdlib=libc++" \
linkflags="-std=c++11 -stdlib=libc++" \
variant=release \
--prefix=$PREFIX_PATH
