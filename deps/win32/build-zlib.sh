#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

WINDOWS_64BIT="x86_64-w64-mingw32"
WINDOWS_32BIT="i686-w64-mingw32"
TARGET_ARCH=$WINDOWS_32BIT

ZLIB_VERSION="zlib-1.2.8"
ZLIB_TARBALL="${ZLIB_VERSION}.tar.gz"

mkdir -p src/

#
# zlib
#
pushd src
if [ ! -e "${ZLIB_TARBALL}"]
then
  rm -fr zlib/

  if wget "http://zlib.net/${ZLIB_TARBALL}"
  then
    echo "Downloaded zlib"
  else
    rm ${ZLIB_TARBALL}
    echo "Failed to download zlib"
    exit 1
  fi
fi
popd

pushd src
if [ ! -e "zlib" ]
then
    if tar -xzvf "${ZLIB_TARBALL}"
    then
      mv ${ZLIB_VERSION} zlib/
      cd zlib/
      CC=${TARGET_ARCH}-gcc AR=${TARGET_ARCH}-ar RANLIB=${TARGET_ARCH}-ranlib ./configure --prefix=/usr/${TARGET_ARCH} --static
      if [ $? -ne 0 ]; then
        echo "Failed to configure zlib"
        cd ../
        rm -fr zlib/
        exit 1
      fi
      make
      if [ $? -ne 0 ]; then
        echo "Failed to build zlib"
        cd ../
        rm -fr zlib/
        exit 1
      fi
      sudo make install
      if [ $? -ne 0 ]; then
        echo "Failed to install zlib"
        cd ../
        rm -fr zlib/
        exit 1
      fi
    else
      rm -fr ${ZLIB_VERSION}
      rm ${ZLIB_TARBALL}
      echo "Failed to extract ${ZLIB_TARBALL}"
      exit 1
    fi
fi
popd
