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

rm -fr dist/

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

  # download boost
  echo "Downloding ${LIB_BOOST_TARBALL}"
  if wget -O ${LIB_BOOST_TARBALL} "http://sourceforge.net/projects/boost/files/boost/${LIB_BOOST_VERSION}/${LIB_BOOST_TARBALL}/download"
  then
      echo "Download Successful"
  else
      "Downloading Boost (${LIB_BOOST_TARBALL}) Failed"
      rm ${LIB_BOOST_TARBALL}
      exit 1
  fi
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
if [ ! -e "${LIBTORRENT_TARBALL}" ]
then
    rm -fr libtorrent/

    # download libtorrent
    echo "Downloding ${LIBTORRENT_TARBALL}"
    if wget -O ${LIBTORRENT_TARBALL} "https://github.com/arvidn/libtorrent/archive/${LIBTORRENT_TARBALL}"
    then
        echo "Downloadin Successful"
    else
        echo "Downloading libtorrent failed"
        rm ${LIBTORRENT_TARBALL}
        exit 1
    fi
fi

if [ ! -e "libtorrent" ]
then
  if tar -xzvf ${LIBTORRENT_TARBALL}
  then
      mv libtorrent-${LIBTORRENT_VERSION}/ libtorrent
      cp "../user-config-${TARGET_ARCH}.jam" libtorrent/user-config.jam
      cd libtorrent/
      #patch 1 MB packet limit
      patch src/bt_peer_connection.cpp ../../libtorrent-patch.diff

      #patch case sensitive include files
      sed -ie "s/^#include <Windows.h>/#include <windows.h>/" ed25519/src/seed.cpp
      sed -ie "s/^#include <Wincrypt.h>/#include <wincrypt.h>/" ed25519/src/seed.cpp
  else
      echo "Failed Extracting Libtorrent"
      rm ${LIBTORRENT_TARBALL}
      rm -fr libtorrent-${LIBTORRENT_VERSION}/
      exit 1
  fi
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

  if [ ! -e "odb_2.4.0-1_amd64.deb" ]; then
    if wget http://www.codesynthesis.com/download/odb/2.4/odb_2.4.0-1_amd64.deb
    then
      echo "Downloaded ODB compiler"
    else
      rm odb_2.4.0-1_amd64.deb
      echo "Failed to download ODB compiler"
      exit 1
    fi
  fi

  sudo dpkg -i odb_2.4.0-1_amd64.deb
  if [ $? -ne 0 ]; then
    echo "Failed to install odb compiler"
    exit 1
  fi
fi

# ODB Common Runtime Library
if [ ! -e "libodb-2.4.0.tar.bz2" ]
then
    rm -fr libodb-2.4.0/

    if wget http://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2
    then
        echo "Downloaded ODB common runtime library"
    else
        echo "Failed to download ODB common runtime library"
        rm libodb-2.4.0.tar.bz2
        exit 1
    fi
fi

if [ ! -e "libodb-2.4.0" ]
then
    if tar -xjvf libodb-2.4.0.tar.bz2
    then
        cd libodb-2.4.0
        ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
          CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to Build libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        sudo make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        cd ..
    else
        echo "failed to extract libodb-2.4.0.tar.bz2"
        rm libodb-2.4.0.tar.bz2
        rm -fr libodb-2.4.0
        exit 1
    fi
fi

if [ ! -e "libodb-sqlite-2.4.0.tar.bz2" ]
then
    rm -fr libodb-sqlite-2.4.0/

    if wget http://www.codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2
    then
        echo "Downloaded ODB sqlite runtime library"
    else
        echo "Failed to download ODB sqlite runtime library"
        rm libodb-sqlite-2.4.0.tar.bz2
        exit 1
    fi
fi

if [ ! -e "libodb-sqlite-2.4.0" ]
then
    if tar -xjvf libodb-sqlite-2.4.0.tar.bz2
    then
        cd libodb-sqlite-2.4.0
        ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
          CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to build libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
        sudo make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
    else
        echo "Failed to extract libodb-sqlite-2.4.0.tar.bz2"
        rm libodb-sqlite-2.4.0.tar.bz2
        rm -fr libodb-sqlite-2.4.0
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

pushd src
if [ ! -e "${LIBPNG_TARBALL}" ]
then
    rm -fr ${LIBPNG_VERSION}

    if wget -O ${LIBPNG_TARBALL} "ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/${LIBPNG_TARBALL}"
    then
        echo "Download Successful"
    else
        echo "Downloading ${LIBPNG_TARBALL} failed"
        rm ${LIBPNG_TARBALL}
        exit 1
    fi
fi

if [ ! -e "${LIBPNG_VERSION}" ]
then
    if tar -xzvf "${LIBPNG_TARBALL}"
    then
        cd "${LIBPNG_VERSION}"/
        ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} \
            CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
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
./configure --host=${TARGET_ARCH} --prefix=/usr/${TARGET_ARCH} --without-tools --enable-static --disable-shared
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA qrencode"
    exit 1
fi
sudo make install
cd ../stdutils
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

#optionally build full mSIGNA app
#cd ../../
#./build-all.sh mingw32
