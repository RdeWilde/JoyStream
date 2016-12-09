# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 6 2015

#gcc 6.1 fix
INCLUDEPATH += $$QMAKE_DEFAULT_INCDIRS

DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE
DEFINES += TORRENT_LINKING_STATIC

DEFINES += BOOST_EXCEPTION_DISABLE
DEFINES += BOOST_ASIO_ENABLE_CANCELIO
DEFINES += BOOST_SYSTEM_STATIC_LINK=1
DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021

DEFINES += BOOST_ASIO_SEPARATE_COMPILATION

# Windows - cross compiled using mingw64
win32 {
    include(conf-mingw-32.pri)
}

# Unix
unix:!macx {
    include(conf-linux.pri)
}

# Mac OSX
macx {
    include(conf-osx.pri)
}

# libtorrent
CONFIG(release, debug|release) {

    # Added for the benefit of QtCreator only, next lone does the same thing
    INCLUDEPATH += $$LIBTORRENT_LOCATION/release/include
    #QMAKE_CXXFLAGS += -isystem $$LIBTORRENT_LOCATION/release/include

    LIBS += -L$$LIBTORRENT_LOCATION/release/lib -ltorrent
    DEFINES += NDEBUG
    DEFINES += TORRENT_DISABLE_LOGGING TORRENT_USE_ASSERTS=0
} else {

    # Added for the benefit of QtCreator only, next lone does the same thing
    INCLUDEPATH += $$LIBTORRENT_LOCATION/debug/include
    #QMAKE_CXXFLAGS += -isystem $$LIBTORRENT_LOCATION/debug/include

    LIBS += -L$$LIBTORRENT_LOCATION/debug/lib -ltorrent
    DEFINES += TORRENT_DEBUG
    DEFINES += TORRENT_LOGGING
    DEFINES += TORRENT_VERBOSE_LOGGING
}

# mSIGNA
# Added for the benefit of QtCreator only, next line does the same thing
INCLUDEPATH += $$MSIGNA_SYSROOT/include
#QMAKE_CXXFLAGS += -isystem $$MSIGNA_SYSROOT/include
LIBS += -L$$MSIGNA_SYSROOT/lib -lCoinQ -llogger -lCoinCore

# Boost
LIBS += \
 -lboost_system$$BOOST_LIB_SUFFIX \
 -lboost_filesystem$$BOOST_LIB_SUFFIX \
 -lboost_regex$$BOOST_LIB_SUFFIX \
 -lboost_thread$$BOOST_THREAD_LIB_SUFFIX$$BOOST_LIB_SUFFIX \
 -lboost_serialization$$BOOST_LIB_SUFFIX \
 -lboost_chrono$$BOOST_LIB_SUFFIX \
 -lboost_random$$BOOST_LIB_SUFFIX

#winsock libs must come after libtorrent
win32 {
  LIBS += -lws2_32 -lmswsock
}

#odb runtime
LIBS += -lodb-sqlite -lodb

# pthreads and zlib
LIBS += -lpthread -lz

# openssl
LIBS += -lcrypto -lssl 

#libgdi must come after libcrypto
win32 {
  LIBS += -lgdi32
}

LIBS += -lsqlite3
