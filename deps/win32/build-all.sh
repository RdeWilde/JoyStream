#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

WINDOWS_64BIT="x86_64-w64-mingw32"
WINDOWS_32BIT="i686-w64-mingw32"
TARGET_ARCH=$WINDOWS_32BIT

LIB_BOOST_VER="boost_1_59_0"
LIB_BOOST_TARBALL="${LIB_BOOST_VER}.tar.gz"
LIB_BOOST_VERSION="1.59.0"

LIBTORRENT_VERSION="libtorrent-1_0_7"
LIBTORRENT_TARBALL="${LIBTORRENT_VERSION}.tar.gz"

LIBPNG_VERSION="libpng-1.6.21"
LIBPNG_TARBALL="${LIBPNG_VERSION}.tar.gz"

ZLIB_VERSION="zlib-1.2.8"
ZLIB_TARBALL="${ZLIB_VERSION}.tar.gz"

OPENSSL_VERSION="openssl-1.0.2g"
OPENSSL_TARBALL="${OPENSSL_VERSION}.tar.gz"

SQLITE_VERSION="sqlite-autoconf-3090200"
SQLITE_TARBALL="${SQLITE_VERSION}.tar.gz"

mkdir -p src/
mkdir -p dist/

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

if [ ! -e "zlib" ]
then
    if tar -xzvf "${ZLIB_TARBALL}"
    then
      mv ${ZLIB_VERSION} zlib/
      pushd zlib/
      CC=${TARGET_ARCH}-gcc AR=${TARGET_ARCH}-ar RANLIB=${TARGET_ARCH}-ranlib ./configure --prefix=/usr/${TARGET_ARCH} --static
      if [ $? -ne 0 ]; then
        echo "Failed to configure zlib"
        popd
        rm -fr zlib/
        exit 1
      fi
      make
      if [ $? -ne 0 ]; then
        echo "Failed to build zlib"
        popd
        rm -fr zlib/
        exit 1
      fi
      sudo make install
      if [ $? -ne 0 ]; then
        echo "Failed to install zlib"
        popd
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

#
# OpenSSL
#
pushd src
if [ ! -e "${OPENSSL_TARBALL}" ]
then
  # if we are downloading new version remove old build
  rm -fr openssl/

  # download openssl
  echo "Downloding ${OPENSSL_TARBALL}"
  if wget -O ${OPENSSL_TARBALL} "https://www.openssl.org/source/${OPENSSL_TARBALL}"
    then
        echo "Download Successful"
    else
        rm ${OPENSSL_TARBALL};
        exit 1
  fi
fi

if [ ! -r "openssl" ]
then
    if tar -xzvf "${OPENSSL_TARBALL}"
    then
        mv ${OPENSSL_VERSION}/ openssl
        pushd openssl/
        CROSS_COMPILE="${TARGET_ARCH}-" ./Configure mingw64 no-asm no-shared --prefix=/usr/${TARGET_ARCH}
        if [ $? -ne 0 ]; then
            echo "Failed to configure openssl"
            popd
            rm -fr openssl/
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Building OpenSSL failed";
            popd
            rm -fr openssl/
            exit 1
        fi
        sudo make install_sw
        if [ $? -ne 0 ]; then
            echo "Installing OpenSSL failed";
            popd
            rm -fr openssl/
            exit 1
        fi
        popd
    else
        echo "Failed Extracting ${OPENSSL_TARBALL}";
        rm ${OPENSSL_TARBALL}
        rm -fr ${OPENSSL_VERSION}/
        exit 1
    fi
fi
popd

#
# sqlite
#
pushd src
if [ ! -e "${SQLITE_TARBALL}" ]
  then
  rm -fr sqlite/

  if wget "https://www.sqlite.org/2015/${SQLITE_TARBALL}"
    then
      echo "Download Successful"
    else
      rm ${SQLITE_TARBALL}
      echo "Failed to download ${SQLITE_TARBALL}"
      exit 1
    fi
fi

if [ ! -e "sqlite" ]
then
    if tar -xzvf "${SQLITE_TARBALL}"
    then
      mv "${SQLITE_VERSION}/" sqlite/
      pushd sqlite
      ./configure --host=${TARGET_ARCH} --target=windows --prefix=/usr/${TARGET_ARCH} CFLAGS=-DSQLITE_ENABLE_UNLOCK_NOTIFY
      if [ $? -ne 0 ]; then
        echo "Failed to configure sqlite"
        popd
        rm -fr sqlite/
        exit 1
      fi
      make
      if [ $? -ne 0 ]; then
        echo "Failed to build sqlite"
        popd
        rm -fr sqlite/
        exit 1
      fi
      sudo make install
      if [ $? -ne 0 ]; then
        echo "Failed to install sqlite"
        popd
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

./build-qt-release-i686-w64-mingw32.sh
if [ $? -ne 0 ]; then
  echo "Failed to build Qt"
  exit 1
fi

#
# Boost
#
pushd src
if [ ! -e "${LIB_BOOST_TARBALL}" ]
then
  # remove old build
  rm -fr boost/

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

./build-boost-${TARGET_ARCH}.sh
if [ $? -ne 0 ]; then
    echo "Failed to Build Boost"
    exit 1
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
        sudo make install
    else
        echo "Failed to build libpng"
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
