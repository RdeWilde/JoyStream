#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

PREFIX_PATH=$PWD/dist/release

cd src/boost

./b2 install \
  toolset=gcc \
  address-model=64 \
  target-os=linux \
  threading=multi \
  link=static \
  --without-mpi \
  --without-python \
  -sNO_BZIP2=1 \
  --layout=tagged \
  variant=release \
  cxxflags="-std=gnu++11" \
  --prefix=$PREFIX_PATH

# Install to system /usr/local
sudo cp $PREFIX_PATH/lib/libboost_* /usr/local/lib/
sudo cp -R $PREFIX_PATH/include/boost /usr/local/include/

#--layout=tagged  will add the -mt suffix if multithreading
# note mSIGNA build on linux doesn't look for tagged libs
# so its deps will build, but not mSIGNA app, unless another boost installation exists
