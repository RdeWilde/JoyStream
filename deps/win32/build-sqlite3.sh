#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

WINDOWS_64BIT="x86_64-w64-mingw32"
WINDOWS_32BIT="i686-w64-mingw32"
TARGET_ARCH=$WINDOWS_32BIT

. ../thirdparty-libs/versions.sh
THIRDPARTY=`pwd`/../thirdparty-libs/

mkdir -p src/

#
# sqlite
#
pushd src
if [ ! -e "${SQLITE_TARBALL}" ]
  then
  rm -fr sqlite/

  cp ${THIRDPARTY}/${SQLITE_TARBALL} ./
fi
popd

pushd src
if [ ! -e "sqlite" ]
then
    if tar -xzvf "${SQLITE_TARBALL}"
    then
      mv "${SQLITE_VERSION}/" sqlite/
      cd sqlite
      ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} CFLAGS=-DSQLITE_ENABLE_UNLOCK_NOTIFY
      if [ $? -ne 0 ]; then
        echo "Failed to configure sqlite"
        cd ../
        rm -fr sqlite/
        exit 1
      fi
      make
      if [ $? -ne 0 ]; then
        echo "Failed to build sqlite"
        cd ../
        rm -fr sqlite/
        exit 1
      fi
      sudo make install
      if [ $? -ne 0 ]; then
        echo "Failed to install sqlite"
        cd ../
        rm -fr sqlite/
        exit 1
      fi
    else
      echo "Failed to extract sqlite"
      rm -fr ${SQLITE_VERSION}
      rm ${SQLITE_TARBALL}
      exit 1
    fi
fi
popd
