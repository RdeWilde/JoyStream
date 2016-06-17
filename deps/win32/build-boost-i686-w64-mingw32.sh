#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

PREFIX_PATH=$PWD/dist/release

cd src/boost

./b2 install \
  toolset=gcc \
  address-model=32 \
  target-os=windows \
  threading=multi \
  tjreadapi=win32 \
  link=static \
  runtime-link=static \
  --user-config=user-config.jam \
  --without-mpi \
  --without-python \
  --without-context \
  -sNO_BZIP2=1 \
  --layout=tagged \
  variant=release \
  cxxflags="-std=gnu++11" \
  --prefix=$PREFIX_PATH

# Install to system /usr/i686-w64-mingw32
sudo cp $PREFIX_PATH/lib/libboost_* /usr/i686-w64-mingw32/lib/
sudo cp -R $PREFIX_PATH/include/boost /usr/i686-w64-mingw32/include/

