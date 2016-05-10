#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

cd src/boost
#build without icu (unicode support needed for regex)
#sudo ./b2 toolset=gcc address-model=64 target-os=linux threading=multi  \
#        --without-mpi --without-python -sNO_BZIP2=1 --layout=tagged --disable-icu variant=release install

sudo ./b2 toolset=gcc address-model=64 target-os=linux --layout=tagged \
  threading=multi link=static \
  --without-mpi --without-python -sNO_BZIP2=1 variant=release \
  cxxflags="-std=gnu++11" \
  --prefix=/usr/local/ install

#--layout=tagged    will add the -mt suffix if multithreading
