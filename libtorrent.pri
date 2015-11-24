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

DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE
DEFINES += BOOST_EXCEPTION_DISABLE
DEFINES += BOOST_ASIO_ENABLE_CANCELIO

# Windows
win32 {

    LIBTORRENT_LOCATION = /home/mokhtar/JoyStream-32bit/libtorrent-libtorrent-1_0_5
    INCLUDEPATH += $$LIBTORRENT_LOCATION/include

    # LIBTORRENT DEFINES
    DEFINES += TORRENT_LINKING_STATIC

    DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
    DEFINES += BOOST_ASIO_SEPARATE_COMPILATION

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

    LIBTORRENT_LOCATION = $$PWD/deps/linux/dist

    INCLUDEPATH += $$LIBTORRENT_LOCATION/release/include

    # Linking
    CONFIG(release, debug|release) {
	LIBS += -L$$LIBTORRENT_LOCATION/release/lib -ltorrent
    } else {
	LIBS += -L$$LIBTORRENT_LOCATION/debug/lib -ltorrent
        DEFINES += TORRENT_LOGGING
        DEFINES += TORRENT_VERBOSE_LOGGING
    }

}

# Mac
macx {

    LIBTORRENT_LOCATION= $$PWD/deps/osx/dist
    INCLUDEPATH += $$LIBTORRENT_LOCATION/release/include

    # Linking
    CONFIG(release, debug|release) {
	LIBS += -L$$LIBTORRENT_LOCATION/release/lib -ltorrent
    } else {
	LIBS += -L$$LIBTORRENT_LOCATION/debug/lib -ltorrent
    }
}

