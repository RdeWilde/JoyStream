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

#If we remove the dist folder then everytime we run this script, it will
#cause all libraries that depend on libtorrent to be recompiled which is not necessary
#if the libtorrent headers and source haven't changed
#rm -fr dist/

mkdir -p src/
mkdir -p dist/

./build-zlib.sh
if [ $? -ne 0 ]; then
  exit 1
fi

./build-openssl.sh
if [ $? -ne 0 ]; then
  exit 1
fi

./build-sqlite3.sh
if [ $? -ne 0 ]; then
  exit 1
fi

#
# Boost
#
pushd src
if [ ! -e "${LIB_BOOST_TARBALL}" ]
then
  # remove old build
  sudo rm -fr boost/

  cp ${THIRDPARTY}/${LIB_BOOST_TARBALL} ./
fi

if [ ! -e "boost" ]
then
  if tar -xzvf "${LIB_BOOST_TARBALL}"
  then
    mv ${LIB_BOOST_VER}/ boost
    cp "../user-config-${TARGET_ARCH}.jam" boost/user-config.jam
    cd boost/
    ./bootstrap.sh --without-icu
    if [ $? -ne 0 ]; then
        echo "Failed to bootstrap Boost"
        cd ../
        sudo rm -fr ${LIB_BOOST_VER}/
        exit 1
    fi
  else
    rm ${LIB_BOOST_TARBALL}
    sudo rm -fr ${LIB_BOOST_VER}/
    exit 1
  fi
fi
popd

./build-boost-${TARGET_ARCH}.sh
if [ $? -ne 0 ]; then
    echo "Failed to Build Boost, continuing anyway"
    # Skipping this check for now, boost context module fails to build with mingw64
    # because it needs microsoft masm (assembler) and the --without-context flag doesn't
    # seem to work
    #exit 1
fi

#
# libtorrent
#
pushd src
mkdir -p libtorrent-joystream/
if rsync -rtvu ${THIRDPARTY}/libtorrent/ libtorrent-joystream/
then
    cp "../user-config-${TARGET_ARCH}.jam" libtorrent-joystream/user-config.jam
    echo "Copied joystream libtorrent repo Successfuly"
else
    echo "Failed to copy joystream libtorrent repo"
    exit 1
fi
popd

./build-libtorrent-${TARGET_ARCH}.sh
if [ $? -ne 0 ]; then
    echo "Failed to build Libtorrent"
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
        ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
          CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
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

if [ ! -e "${ODB_SQLITE_RUNTIME}" ]
then
    if tar -xjvf ${ODB_SQLITE_RUNTIME_TARBALL}
    then
        cd ${ODB_SQLITE_RUNTIME}
        ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
          CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
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


#ODB will, by default, use the linux headers. We want this behavior for system libraries, but we want to override it for
#nonsystem libraries. To do this, we create symlinks.
pushd /usr/${TARGET_ARCH}
sudo mkdir -p local/include
cd local/include
sudo ln -s ../../include/odb/ odb
sudo ln -s ../../include/boost/ boost
sudo ln -s ../../include/openssl/ openssl
sudo ln -s ../../include/sqlite3.h sqlite3.h
sudo ln -s ../../include/sqlite3ext.h sqlite3ext.h
popd

#mSIGNA (bulding only required libraries for JoyStream)
pushd src
mkdir -p mSIGNA-joystream/
if rsync -rtvu ${THIRDPARTY}/mSIGNA/ mSIGNA-joystream/
then
    echo "Copied joystream mSGINA repo Successfuly"
else
    echo "Failed to copy joystream mSIGNA repo"
    exit 1
fi

cd mSIGNA-joystream/

cd deps/stdutils
OS=mingw32 SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA stdutils"
    exit 1
fi
cd ../logger
OS=mingw32 make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA logger"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../Signals
OS=mingw32 SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA Signals"
    exit 1
fi
cd ../CoinCore
OS=mingw32 make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinCore"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../CoinQ
OS=mingw32 make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinQ"
    exit 1
fi
SYSROOT=../../sysroot make install
popd
#optionally build full mSIGNA app
#cd ../../
#./build-all.sh mingw32
