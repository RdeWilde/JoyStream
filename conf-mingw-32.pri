# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += WIN32
DEFINES += _WIN32

#INCLUDEPATH += /usr/i686-w64-mingw32/include
QMAKE_CXXFLAGS += -isystem /usr/i686-w64-mingw32/include

LIBS += -L/usr/i686-w64-mingw32/lib

LIBS += \
    -L/usr/i686-w64-mingw32/plugins/platforms \
    -L/usr/i686-w64-mingw32/plugins/imageformats \
    -L/usr/i686-w64-mingw32/plugins/bearer \
    -L/usr/i686-w64-mingw32/plugins/sqldrivers

#statically link the runtime
LIBS += -static-libgcc -static-libstdc++ -static

# libtorrent
LIBTORRENT_LOCATION= $$PWD/deps/win32/dist

# mSIGNA
MSIGNA_SYSROOT = $$PWD/deps/win32/src/mSIGNA/sysroot

# Boost
# boost was build with --layout=tagged option so libraries are suffixed (mt=multithreaded, s=static, d=debug)

BOOST_LIB_SUFFIX = -mt-s

BOOST_THREAD_LIB_SUFFIX = _win32
