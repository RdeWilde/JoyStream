#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

LIB_BOOST_VER="boost_1_59_0"
LIB_BOOST_TARBALL="${LIB_BOOST_VER}.tar.gz"
LIB_BOOST_VERSION="1.59.0"

LIBTORRENT_VERSION="libtorrent-1_0_7"
LIBTORRENT_TARBALL="${LIBTORRENT_VERSION}.tar.gz"

mkdir -p src/
mkdir -p dist/

pushd src
if [ ! -e "boost" ]
then
  if [ ! -e "${LIB_BOOST_TARBALL}" ]
  then
    # download boost
    echo "Downloding ${LIB_BOOST_TARBALL}"
    wget -O ${LIB_BOOST_TARBALL} "http://sourceforge.net/projects/boost/files/boost/${LIB_BOOST_VERSION}/${LIB_BOOST_TARBALL}/download"
  fi

  tar -xzvf "${LIB_BOOST_TARBALL}"
  mv ${LIB_BOOST_VER}/ boost

  cd boost/
  ./bootstrap.sh
  cd ../../
  ./build-boost.sh
fi
popd

pushd src
if [ ! -e "${LIBTORRENT_TARBALL}" ]
then
  # download libtorrent
  rm -fr libtorrent/
  echo "Downloding ${LIBTORRENT_TARBALL}"
  wget -O ${LIBTORRENT_TARBALL} "https://github.com/arvidn/libtorrent/archive/${LIBTORRENT_TARBALL}"

  tar -xzvf ${LIBTORRENT_TARBALL}
  mv libtorrent-${LIBTORRENT_VERSION}/ libtorrent
  cd libtorrent/
  patch src/bt_peer_connection.cpp ../../libtorrent-patch.diff
fi
popd

if [ "$BUILD_TYPE" == "DEBUG" ]
  then
      ./build-libtorrent-debug.sh
  else
      ./build-libtorrent.sh
fi

pushd src
if [ ! -e "odb" ]
then
    mkdir -p odb
    cd odb
    # ODB Compiler
    if [ ! -e "odb_2.4.0-1_amd64.deb" ]
    then
        wget http://www.codesynthesis.com/download/odb/2.4/odb_2.4.0-1_amd64.deb
    fi
    sudo dpkg -i odb_2.4.0-1_amd64.deb

    # Common Runtime Library
    if [ ! -e "libodb-2.4.0.tar.bz2" ]
    then
        wget http://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2 ;
    fi
    tar -xjvf libodb-2.4.0.tar.bz2
    cd libodb-2.4.0
    ./configure
    make
    sudo make install
    cd ..

    # Database Runtime Library
    if [ ! -e "libodb-sqlite-2.4.0.tar.bz2" ]
    then
        wget http://www.codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2
    fi
    tar -xjvf libodb-sqlite-2.4.0.tar.bz2
    cd libodb-sqlite-2.4.0
    ./configure
    make
    sudo make install
fi
popd

pushd src
if [ ! -e "libpng-1.6.19" ]
then
    if [ ! -e "libpng-1.6.19.tar.gz" ]
    then
        wget ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/libpng-1.6.19.tar.gz
    fi
    tar -xzvf libpng-1.6.19.tar.gz
    cd libpng-1.6.19/
    make
    sudo make install
fi
popd

#mSIGNA (bulding only required libraries for JoyStream)
pushd src
if [ ! -e "mSIGNA" ]
then
    git clone https://github.com/ciphrex/mSIGNA.git
    cd mSIGNA/
    git checkout -b v0.9.6 v0.9.6
    cd deps/qrencode-3.4.3
    make
    make install
    cd ../stdutils
    SYSROOT=../../sysroot make install
    cd ../logger
    make
    SYSROOT=../../sysroot make install
    cd ../Signals
    SYSROOT=../../sysroot make install
    cd ../CoinCore
    make
    SYSROOT=../../sysroot make install
    cd ../CoinQ
    make
    SYSROOT=../../sysroot make install

    #optionally build full mSIGNA app
    #cd ../../
    #./build-all.sh linux
fi
popd
