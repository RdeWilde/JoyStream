# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

LIBTORRENT_NAME = libtorrent-rasterbar-1.0.5

# Windows
win32 {

    LIBTORRENT_LOCATION = C:/$$LIBTORRENT_NAME

    DEFINES += WIN32

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/release/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent
    } else {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/debug/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent
        LIBS += DbgHelp.lib # The mailinglist suggested this to be able to debug with MSVC, try LIBS += libdbghelp for mingw
    }
}

# Unix
unix:!macx {

    LIBTORRENT_LOCATION = /home/bedeho/JoyStream/Development/libs/$$LIBTORRENT_NAME

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-4.8/release/address-model-64/deprecated-functions-off/link-static/threading-multi -llibtorrent
    } else {
        LIBS += -L$$LIBTORRENT_LOCATION/bin/gcc-4.8/debug/address-model-64/deprecated-functions-off/link-static/threading-multi -llibtorrent
    }
}

INCLUDEPATH += $$LIBTORRENT_LOCATION/include

DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE

CONFIG(release, debug|release) {
    DEFINES += NDEBUG
} else {
    DEFINES += TORRENT_DEBUG
}
