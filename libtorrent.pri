# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

# common to all platform
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
} else {
    DEFINES += TORRENT_DEBUG
}

# Windows
win32 {

    LIBTORRENT_LOCATION = /home/mokhtar/JoyStream-32bit/libtorrent-libtorrent-1_0_5

    # LIBTORRENT DEFINES
    DEFINES += TORRENT_LINKING_STATIC
    DEFINES += TORRENT_DISABLE_GEO_IP

    DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
    DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
    DEFINES += BOOST_ASIO_ENABLE_CANCELIO
    DEFINES += BOOST_EXCEPTION_DISABLE

    DEFINES += BOOST_SYSTEM_STATIC_LINK=1

    DEFINES += UNICODE
    DEFINES += _UNICODE
    DEFINES += WIN32
    DEFINES += _WIN32

 CONFIG(release, debug|release) {
    LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-mingw-mingw32/release/address-model-32/link-static/runtime-link-static/target-os-windows/threadapi-win32/threading-multi/ -ltorrent
 } else {
    LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-mingw-mingw32/debug/address-model-32/link-static/runtime-link-static/target-os-windows/threadapi-win32/threading-multi/ -ltorrent
 }
}

# Unix
unix:!macx {

    LIBTORRENT_LOCAL_BUILD = /home/bedeho/JoyStream/Development/libs/libtorrent-rasterbar-1.0.5

    INCLUDEPATH += $$LIBTORRENT_LOCAL_BUILD/include

    CONFIG(release, debug|release) {
        LIBS += -L$$LIBTORRENT_LOCAL_BUILD/bin/gcc-4.9.2/release/address-model-64/boost-link-shared/deprecated-functions-off/link-static/threading-multi -ltorrent
    } else {
        #LIBS += -L$$LIBTORRENT_LOCAL_BUILD/bin/gcc-4.9.2/debug/address-model-64/boost-link-shared/deprecated-functions-off/link-static/threading-multi -ltorrent

        LIBS += -L$$LIBTORRENT_LOCAL_BUILD/bin/gcc-4.9.2/debug/address-model-64/boost-link-shared/deprecated-functions-off/link-static/logging-verbose/threading-multi -ltorrent
        DEFINES += TORRENT_LOGGING
        DEFINES += TORRENT_VERBOSE_LOGGING
    }

    DEFINES += TORRENT_DISABLE_LOGGING
    DEFINES += TORRENT_NO_DEPRECATE
    DEFINES += TORRENT_DISABLE_GEO_IP
    DEFINES += BOOST_EXCEPTION_DISABLE
    DEFINES += BOOST_ASIO_ENABLE_CANCELIO
}


        #LIBTORRENT_LOCAL_BUILD = /home/bedeho/JoyStream/Development/libs/libtorrent-build

        #INCLUDEPATH += $$LIBTORRENT_LOCAL_BUILD/include
        # SHARED ==============================
        #LIBS += -L$$LIBTORRENT_LOCAL_BUILD/lib -ltorrent-rasterbar

        #DEFINES += TORRENT_DISABLE_LOGGING
        #DEFINES += TORRENT_USE_OPENSSL
        #DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
        #DEFINES += BOOST_EXCEPTION_DISABLE
        #DEFINES += BOOST_ASIO_ENABLE_CANCELIO
        #DEFINES += TORRENT_LINKING_SHARED
        #DEFINES += TORRENT_NO_DEPRECATE

# Mac
macx {

    LIBTORRENT_LOCATION =/Users/mokhtar/JoyStream/libtorrent-libtorrent-1_0_5

    #DEFINES += TORRENT_LOGGING
    DEFINES += TORRENT_NO_DEPRECATE
    DEFINES += TORRENT_DISABLE_GEO_IP
    DEFINES += BOOST_EXCEPTION_DISABLE
    DEFINES += BOOST_ASIO_ENABLE_CANCELIO

    # Linking
    CONFIG(release, debug|release) {
        #library built with: bjam address-model=64 deprecated-functions=off boost-link=shared boost=source geoip=off link=static asserts=off release
        #LIBS += -L$$LIBTORRENT_LOCATION/bin/darwin-4.2.1/release/address-model-64/asserts-off/boost-link-shared/boost-source/deprecated-functions-off/link-static/threading-multi -ltorrent
        LIBS += -L$$LIBTORRENT_LOCATION/bin/clang-darwin-4.2.1/release/address-model-64/asserts-off/boost-source/deprecated-functions-off/link-static/threading-multi/ -ltorrent

    } else {
        #library built with: bjam address-model=64 deprecated-functions=off boost-link=shared boost=source geoip=off link=static debug
        LIBS += -L$$LIBTORRENT_LOCATION/bin/darwin-4.2.1/debug/address-model-64/boost-link-shared/boost-source/deprecated-functions-off/link-static/threading-multi -ltorrent
   }
}

INCLUDEPATH += $$LIBTORRENT_LOCATION/include
