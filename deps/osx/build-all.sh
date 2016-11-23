#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

#set -x

. ../thirdparty-libs/versions.sh
THIRDPARTY=`pwd`/../thirdparty-libs/

#If we remove the dist folder then everytime we run this script, it will
#cause all libraries that depend on libtorrent to be recompiled which is not necessary
#if the libtorrent headers and source haven't changed
#rm -fr dist/

mkdir -p src/
mkdir -p dist/

pushd src
if [ ! -e "${OPENSSL_TARBALL}" ]
then
  # if we are downloading new version remove old build
  rm -fr openssl/

  cp ${THIRDPARTY}/${OPENSSL_TARBALL} ./
fi

if [ ! -r "openssl" ]
then
    if tar -xzvf "${OPENSSL_TARBALL}"
    then
        mv ${OPENSSL_VERSION}/ openssl
        pushd openssl/
        # https://www.openssl.org/news/secadv/20160301.txt
        ./Configure shared darwin64-x86_64-cc enable-ssl2 --openssldir=/usr/local/
        if [ $? -ne 0 ]; then
            echo "Failed to configure openssl"
            popd
            rm -fr openssl/
            exit 1
        fi
        sed -ie "s/^CFLAG= -/CFLAG= -mmacosx-version-min=10.7 -/" Makefile
        if [ $? -ne 0 ]; then
            echo "Failed to patch openssl"
            popd
            rm -fr openssl/
            exit 1
        fi
        make depend
        make
        if [ $? -ne 0 ]; then
            echo "Building OpenSSL failed";
            popd
            rm -fr openssl/
            exit 1
        fi
        sudo make install
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

pushd src
if [ ! -e "${LIB_BOOST_TARBALL}" ]
then
  # remove old build
  rm -fr boost/

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
    patch tools/build/src/tools/darwin.jam ../../boost-patch.diff
    if [ $? -ne 0 ]; then
        echo "Failed to patch Boost"
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
mkdir -p libtorrent-joystream/
if rsync -rtvu ${THIRDPARTY}/libtorrent/ libtorrent-joystream/
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
if [ ! -e "${ODB_COMPILER_OSX_TARBALL}" ]
then
    rm -fr ${ODB_COMPILER_OSX}

    cp ${THIRDPARTY}/${ODB_COMPILER_OSX_TARBALL} ./
fi

if [ ! -e "${ODB_COMPILER_OSX}" ]
then
    if tar -xjvf ${ODB_COMPILER_OSX_TARBALL}
    then
        echo "export PATH=$(pwd)/${ODB_COMPILER_OSX}/bin:\$PATH" >> ~/.bash_profile
        # http://www.joshstaiger.org/archives/2005/07/bash_profile_vs.html
        source ~/.bash_profile
    else
        echo "Failed to extract ODB compiler ${ODB_COMPILER_OSX_TARBALL}"
        rm ${ODB_COMPILER_OSX_TARBALL}
        rm -fr ${ODB_COMPILER_OSX}
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
        make install
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

# ODB Database Runtime Library
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
        make install
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
OS=osx SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA stdutils"
    exit 1
fi
cd ../logger
OS=osx make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA logger"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../Signals
OS=osx SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA Signals"
    exit 1
fi
cd ../CoinCore
OS=osx make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinCore"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../CoinQ
OS=osx make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinQ"
    exit 1
fi
SYSROOT=../../sysroot make install
popd
#optionally build full mSIGNA app
#cd ../../
#./build-all.sh osx
