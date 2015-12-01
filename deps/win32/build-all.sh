#!/bin/bash
set -x

WINDOWS_64BIT="x86_64-w64-mingw32"
WINDOWS_32BIT="i686-w64-mingw32"
TARGET_ARCH=$WINDOWS_32BIT

LIB_BOOST_VER="boost_1_59_0"
LIB_BOOST_TARBALL="${LIB_BOOST_VER}.tar.gz"
LIB_BOOST_VERSION="1.59.0"

LIBTORRENT_VERSION="libtorrent-1_0_7"
LIBTORRENT_TARBALL="${LIBTORRENT_VERSION}.tar.gz"

mkdir -p src/
mkdir -p dist/

#
# Boost
#
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
  cp "../user-config-${TARGET_ARCH}.jam" boost/user-config.jam
  cd boost/
  ./bootstrap.sh --without-icu
fi
popd
./build-boost-${TARGET_ARCH}.sh

#
# libtorrent
#
pushd src
if [ ! -e "libtorrent" ]
then
  if [ ! -e "${LIBTORRENT_TARBALL}" ]
  then
    # download libtorrent
    echo "Downloding ${LIBTORRENT_TARBALL}"
    wget -O ${LIBTORRENT_TARBALL} "https://github.com/arvidn/libtorrent/archive/${LIBTORRENT_TARBALL}"
  fi

  tar -xzvf ${LIBTORRENT_TARBALL}
  mv libtorrent-${LIBTORRENT_VERSION}/ libtorrent
  cp "../user-config-${TARGET_ARCH}.jam" libtorrent/user-config.jam
  cd libtorrent/

  #patch 1 MB packet limit
  patch src/bt_peer_connection.cpp ../../libtorrent-patch.diff

  #patch case sensitive include files
  sed -ie "s/^#include <Windows.h>/#include <windows.h>/" ed25519/src/seed.cpp
  sed -ie "s/^#include <Wincrypt.h>/#include <wincrypt.h>/" ed25519/src/seed.cpp

fi
popd
./build-libtorrent-${TARGET_ARCH}.sh

#
# ODB
#
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
    ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
        CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
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
    ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} --enable-threads=win32 \
        CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
    make
    sudo make install
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

#pushd src
#if [ ! -e "libpng-1.6.19" ]
#then
#    if [ ! -e "libpng-1.6.19.tar.gz" ]
#    then
#        wget ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/libpng-1.6.19.tar.gz
#    fi
#    tar -xzvf libpng-1.6.19.tar.gz
#    cd libpng-1.6.19/
#    ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} \
#        CPPFLAGS=-I/usr/${TARGET_ARCH}/include LDFLAGS=-L/usr/${TARGET_ARCH}/lib --enable-static --disable-shared
#    make
#    sudo make install
#fi
#popd

#mSIGNA (bulding only required libraries for JoyStream)
pushd src
if [ ! -e "mSIGNA" ]
then
    git clone https://github.com/mnaamani/mSIGNA.git
    cd mSIGNA/
    git checkout -b windows32bit origin/windows32bit
    cd deps/qrencode-3.4.3
    ./configure --host=${TARGET_ARCH} --prefix=/usr/${TARGET_ARCH} --without-tools --enable-static --disable-shared
    make
    sudo make install
    cd ../stdutils
    OS=mingw32 SYSROOT=../../sysroot make install
    cd ../logger
    OS=mingw32 make
    OS=mingw32 SYSROOT=../../sysroot make install
    cd ../Signals
    OS=mingw32 SYSROOT=../../sysroot make install
    cd ../CoinCore
    OS=mingw32 make
    OS=mingw32 SYSROOT=../../sysroot make install
    cd ../CoinQ
    OS=mingw32 make
    OS=mingw32 SYSROOT=../../sysroot make install

    #optionally build full mSIGNA app
    #cd ../../
    #./build-all.sh mingw32
fi
popd
