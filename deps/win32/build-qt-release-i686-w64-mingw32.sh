#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

set -x

TARGET_ARCH="i686-w64-mingw32"

mkdir -p src/

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
#  QT
#
pushd src
if [ ! -e "qt5-win32-build-release" ]
then
    if [ ! -e "qt-everywhere-opensource-src-5.5.0.tar.gz" ]
      then
        wget http://download.qt-project.org/official_releases/qt/5.5/5.5.0/single/qt-everywhere-opensource-src-5.5.0.tar.gz
        tar -xzvf qt-everywhere-opensource-src-5.5.0.tar.gz
        if [ $? -ne 0 ]; then
          echo "Failed to get Qt source"
          rm qt-everywhere-opensource-src-5.5.0.tar.gz
          rm -fr qt-everywhere-opensource-src-5.5.0/
          exit 1
        fi
    fi

    #Fix Qt5.5.0 issues with preprocessor definitions when building with MinGW.
    sed -i -e 's/QT_NO_SESSIONMANAGER/__QT_NO_SESSIONMANAGER/g' \
        ./qt-everywhere-opensource-src-5.5.0/qtbase/src/gui/kernel/qplatformsessionmanager.h \
        ./qt-everywhere-opensource-src-5.5.0/qtbase/src/gui/kernel/qsessionmanager.h

    #Fix Qt (WINBOOL bug) - https://bugreports.qt.io/browse/QTBUG-46463
    #https://codereview.qt-project.org/#/c/115707/2/src/imports/qtcanvas3d/context3d_p.h
    patch ./qt-everywhere-opensource-src-5.5.0/qtcanvas3d/src/imports/qtcanvas3d/context3d_p.h ../qt-winbool-bug-patch.diff

    # Patch IPC support check in build configure script to enable systemsemaphore and sharedmemory support
    sed -i -e 's/ QT_NO_SYSTEMSEMAPHORE QT_NO_SHAREDMEMORY//g' \
        ./qt-everywhere-opensource-src-5.5.0/qtbase/configure

    mkdir qt5-win32-build-release/

    export INSTALLPREFIX=/usr/i686-w64-mingw32
    export OPENSSL_LIBS='-lssl -lcrypto -lgdi32'

    cd qt5-win32-build-release/
    ../qt-everywhere-opensource-src-5.5.0/configure -prefix / -hostprefix $INSTALLPREFIX/host -plugindir /plugins -translationdir /translations \
        -xplatform win32-g++ -device-option CROSS_COMPILE=i686-w64-mingw32- -sysroot $INSTALLPREFIX -opensource -static -qt-pcre \
        -no-icu -no-glib -no-nis -no-cups -no-iconv -no-dbus -nomake examples -nomake tests \
        -no-feature-style-windowsce -no-feature-style-windowsmobile \
        -no-pch -no-compile-examples -no-qml-debug -no-sm \
        -skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -qt-zlib -openssl-linked -release

    make
    sudo make install

    #Patch Qt to work with cmake for static build. Windows uses library naming convention name.lib, MinGW uses libname.a.
    #So we must replace the names:
    cd /usr/i686-w64-mingw32/lib/cmake
    sudo sed -i -e 's/\/\([^\.]*\)\.lib/\/lib\1\.a/g' $(find * -type f)
fi
popd
