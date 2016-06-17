#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

export BOOST_ROOT=$PWD/src/boost/
PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent

$BOOST_ROOT/b2 install \
  toolset=gcc address-model=64 target-os=linux threading=multi link=static  \
  --without-python boost-link=static deprecated-functions=off logging=off asserts=off variant=release \
  cxxflags="-std=gnu++11" \
  --prefix=$PREFIX_PATH
