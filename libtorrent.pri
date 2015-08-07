# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

LIBTORRENT_NAME = libtorrent-rasterbar-1.0.5

# common to all platform
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
} else {
    DEFINES += TORRENT_DEBUG
}

# Windows
win32 {

    LIBTORRENT_LOCATION = C:/$$LIBTORRENT_NAME

    # LIBTORRENT DEFINES
    DEFINES += BOOST_ALL_NO_LIB
    DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
    DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
    DEFINES += BOOST_EXCEPTION_DISABLE
    DEFINES += BOOST_SYSTEM_STATIC_LINK=1
    DEFINES += TORRENT_USE_OPENSSL
    DEFINES += UNICODE
    DEFINES += _UNICODE
    DEFINES += WIN32
    DEFINES += _WIN32
    DEFINES += WIN32_LEAN_AND_MEAN
    DEFINES += _WIN32_WINNT=0x0500
    DEFINES += _WIN32_IE=0x0500
    DEFINES += _CRT_SECURE_NO_DEPRECATE
    DEFINES += _SCL_SECURE_NO_DEPRECATE
    DEFINES += __USE_W32_SOCKETS
    DEFINES += _FILE_OFFSET_BITS=64
    DEFINES += WITH_SHIPPED_GEOIP_H

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/release/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent
    } else {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/debug/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent
        LIBS += DbgHelp.lib # The mailinglist suggested this to be able to debug with MSVC, try LIBS += libdbghelp for mingw
    }
}

LIBTORRENT_LOCATION = /home/bedeho/JoyStream/Development/libs/libtorrent-rasterbar-1.0.5

# Unix
unix:!macx {

    DEFINES += TORRENT_NO_DEPRECATE
    #DEFINES += TORRENT_DISABLE_LOGGING
    #DEFINES += TORRENT_USE_OPENSSL
    DEFINES += TORRENT_DISABLE_GEO_IP

    # Linking
    CONFIG(release, debug|release) {

        #LIBTORRENT_LOCATION = /home/bedeho/JoyStream/Development/libs/libtorrent-release-install

        LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-4.8/release/address-model-64/deprecated-functions-off/link-static/threading-multi -ltorrent
        #LIBS += -L$$LIBTORRENT_LOCATION/lib -llibtorrent-rasterbar

        DEFINES += NDEBUG
    } else {

        #LIBTORRENT_LOCATION = /home/bedeho/JoyStream/Development/libs/libtorrent-debug-install

        LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-4.8/debug/address-model-64/deprecated-functions-off/link-static/threading-multi -ltorrent
        #LIBS += -L$$LIBTORRENT_LOCATION/lib -llibtorrent-rasterbar

        DEFINES += TORRENT_DEBUG
    }
}

INCLUDEPATH += $$LIBTORRENT_LOCATION/include
