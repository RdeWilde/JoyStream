#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

WINDOWS_64BIT="x86_64-w64-mingw32"
WINDOWS_32BIT="i686-w64-mingw32"
TARGET_ARCH=$WINDOWS_32BIT

OPENSSL_VERSION="openssl-1.0.2g"
OPENSSL_TARBALL="${OPENSSL_VERSION}.tar.gz"

mkdir -p src/

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
popd

pushd src
if [ ! -r "openssl" ]
then
    if tar -xzvf "${OPENSSL_TARBALL}"
    then
        mv ${OPENSSL_VERSION}/ openssl
        cd openssl/
        CROSS_COMPILE="${TARGET_ARCH}-" ./Configure mingw64 no-asm no-shared enable-ssl2 --openssldir=/usr/${TARGET_ARCH}
        if [ $? -ne 0 ]; then
            echo "Failed to configure openssl"
            cd ../
            rm -fr openssl/
            exit 1
        fi
        make depend
        make
        if [ $? -ne 0 ]; then
            echo "Building OpenSSL failed";
            cd ../
            rm -fr openssl/
            exit 1
        fi
        sudo make install_sw
        if [ $? -ne 0 ]; then
            echo "Installing OpenSSL failed";
            cd ../
            rm -fr openssl/
            exit 1
        fi
    else
        echo "Failed Extracting ${OPENSSL_TARBALL}";
        rm ${OPENSSL_TARBALL}
        rm -fr ${OPENSSL_VERSION}/
        exit 1
    fi
fi
popd
