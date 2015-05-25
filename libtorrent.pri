
# Required for including libtorrent headers
#LIBTORRENT_LOCATION = C:/libtorrent-rasterbar-0.16.18
#LIBTORRENT_LOCATION = C:/libtorrent-rasterbar-1.0.3
#LIBTORRENT_LOCATION = C:/libtorrent-rasterbar-1.0.4

LIBTORRENT_LOCATION = C:/MODIFIED-libtorrent-rasterbar-1.0.4

INCLUDEPATH += $$LIBTORRENT_LOCATION/include

#
#  IT IS ABSOLUTELY CRITICAL THAT
#  THESE MATCH WHAT WAS USED DURING BUILD!
#

DEFINES += WIN32
DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE

DEFINES += TORRENT_LOGGING


####################################
# Linking with Libtorrent RELEASE
####################################
CONFIG(release, debug|release) {

    DEFINES += NDEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/release/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent
}

####################################
# Linking with Libtorrent DEBUG
####################################
CONFIG(debug, debug|release) {

    DEFINES += TORRENT_DEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/debug/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    # The mailinglist suggested this to be able
    LIBS += DbgHelp.lib
}
