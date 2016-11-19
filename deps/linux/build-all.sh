#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

. ../thirdparty-libs/versions.sh
THIRDPARTY=`pwd`/../thirdparty-libs/

#If we remove the dist folder then everytime we run this script, it will
#cause all libraries that depend on libtorrent to be recompiled which is not necessary
#if the libtorrent headers and source haven't changed
#rm -fr dist/

mkdir -p src/
mkdir -p dist/

pushd src
if [ ! -e "${LIB_BOOST_TARBALL}" ]
then
  # remove old build
  rm -fr boost/

  # copy boost source
  cp ${THIRDPARTY}/${LIB_BOOST_TARBALL} ./
fi

if [ ! -e "boost" ]
then
  if tar -xzvf "${LIB_BOOST_TARBALL}"
  then
    mv ${LIB_BOOST_VER}/ boost
    cd boost/
    ./bootstrap.sh
    if [ $? -ne 0 ]; then
        echo "Failed to bootstrap Boost"
        cd ../
        rm -fr ${LIB_BOOST_VER}/
        exit 1
    fi
  else
    rm ${LIB_BOOST_TARBALL}
    rm -fr ${LIB_BOOST_VER}/
    exit 1
  fi
fi
popd

./build-boost.sh
if [ $? -ne 0 ]; then
    echo "Failed to Build Boost"
    exit 1
fi

pushd src
mkdir -p libtorrent
if rsync -rtv ${THIRDPARTY}/libtorrent/ libtorrent/
then
    echo "Copied joystream libtorrent repo Successfuly"
else
    echo "Failed to copy joystream libtorrent repo"
    exit 1
fi
popd

if [ "$BUILD_TYPE" == "DEBUG" ]
then
    ./build-libtorrent-debug.sh
else
    ./build-libtorrent.sh
fi

if [ $? -ne 0 ]; then
    echo "Failed to Build Libtorrent"
    exit 1
fi

pushd src
# ODB Compiler
dpkg -s odb
if [ $? -ne 0 ]; then
  echo "ODB compiler not installed"

  sudo dpkg -i ${THIRDPARTY}/${ODB_COMPILER_LINUX}
  if [ $? -ne 0 ]; then
    echo "Failed to install odb compiler"
    exit 1
  fi
fi

# ODB Common Runtime Library
if [ ! -e "${ODB_RUNTIME_TARBALL}" ]
then
    rm -fr ${ODB_RUNTIME}

    cp ${THIRDPARTY}/${ODB_RUNTIME_TARBALL} ./
fi

if [ ! -e "${ODB_RUNTIME}" ]
then
    if tar -xjvf ${ODB_RUNTIME_TARBALL}
    then
        cd ${ODB_RUNTIME}
        ./configure
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb"
            cd ../
            rm -fr ${ODB_RUNTIME}
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to Build libodb"
            cd ../
            rm -fr ${ODB_RUNTIME}
            exit 1
        fi
        sudo make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb"
            cd ../
            rm -fr ${ODB_RUNTIME}
            exit 1
        fi
        cd ..
    else
        echo "failed to extract ${ODB_RUNTIME_TARBALL}"
        rm ${ODB_RUNTIME_TARBALL}
        rm -fr ${ODB_RUNTIME}
        exit 1
    fi
fi

if [ ! -e "${ODB_SQLITE_RUNTIME_TARBALL}" ]
then
    rm -fr ${ODB_SQLITE_RUNTIME}

    cp ${THIRDPARTY}/${ODB_SQLITE_RUNTIME_TARBALL} ./
fi

if [ ! -e ${ODB_SQLITE_RUNTIME} ]
then
    if tar -xjvf ${ODB_SQLITE_RUNTIME_TARBALL}
    then
        cd ${ODB_SQLITE_RUNTIME}
        ./configure
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb-sqlite"
            cd ../
            rm -fr ${ODB_SQLITE_RUNTIME}
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to build libodb-sqlite"
            cd ../
            rm -fr ${ODB_SQLITE_RUNTIME}
            exit 1
        fi
        sudo make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb-sqlite"
            cd ../
            rm -fr ${ODB_SQLITE_RUNTIME}
            exit 1
        fi
    else
        echo "Failed to extract ${ODB_SQLITE_RUNTIME_TARBALL}"
        rm ${ODB_SQLITE_RUNTIME_TARBALL}
        rm -fr ${ODB_SQLITE_RUNTIME}
        exit 1
    fi
fi
popd

pushd src
if [ ! -e "${LIBPNG_TARBALL}" ]
then
    rm -fr ${LIBPNG_VERSION}

    cp ${THIRDPARTY}/${LIBPNG_TARBALL} ./
fi

if [ ! -e "${LIBPNG_VERSION}" ]
then
    if tar -xzvf "${LIBPNG_TARBALL}"
    then
        cd "${LIBPNG_VERSION}"/
        ./configure
        make
        if [ $? -ne 0 ]; then
          echo "Failed to build libpng"
          cd ../
          rm -fr ${LIBPNG_VERSION}
          exit 1
        fi
        sudo make install
    else
        echo "Failed to extract libpng"
        rm ${LIBPNG_TARBALL}
        rm -fr ${LIBPNG_VERSION}
        exit 1
    fi
fi
popd

#mSIGNA (bulding only required libraries for JoyStream)
pushd src
if [ ! -e "mSIGNA" ]
then
    if git clone https://github.com/JoyStream/mSIGNA
    then
        echo "Cloned mSIGNA Successfuly"
    else
        echo "Failed to clone mSIGNA repo"
        rm -fr mSIGNA
        exit 1
    fi
fi

cd mSIGNA/
git checkout joystream-master
if [ $? -ne 0 ]; then
    echo "Local modification of mSIGNA branch, please resolve and try again"
fi
git pull origin joystream-master
if [ $? -ne 0 ]; then
    echo "Problem getting latest mSIGNA commits, please resolve and try again"
fi
cd deps/qrencode-3.4.3
./configure
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA qrencode"
    exit 1
fi
sudo make install
cd ../stdutils
OS=linux SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA stdutils"
    exit 1
fi
cd ../logger
OS=linux make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA logger"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../Signals
OS=linux SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA Signals"
    exit 1
fi
cd ../CoinCore
OS=linux make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinCore"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../CoinQ
OS=linux make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinQ"
    exit 1
fi
SYSROOT=../../sysroot make install

#optionally build full mSIGNA app
#cd ../../
#qmake CONFIG+=release BOOST_LIB_SUFFIX=-mt && make OS=linux
