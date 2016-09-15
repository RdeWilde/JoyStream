#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

export BOOST_ROOT=$PWD/src/boost/
PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent

$PWD/../boost/b2 install \
  toolset=gcc \
  address-model=32 \
  target-os=windows \
  threading=multi \
  threadapi=win32 \
  link=static \
  runtime-link=static \
  --user-config=user-config.jam \
  --without-mpi \
  --without-python \
  -sNO_BZIP2=1 \
  boost-link=static \
  deprecated-functions=off \
  logging=off \
  asserts=off \
  --layout=tagged \
  variant=release \
  cxxflags="-std=gnu++11 -DBOOST_ASIO_SEPARATE_COMPILATION" \
  --prefix=$PREFIX_PATH
