
# Manually adjust these values

BOOST_LOCATION = C:/boost_1_56_0
LIBTORRENT_LOCATION = C:/libtorrent-rasterbar-1.0.2

###########################################################

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/lib
INCLUDEPATH += $$BOOST_LOCATION
INCLUDEPATH += $$LIBTORRENT_LOCATION/include

DEFINES += WIN32
DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE

LIBS += -L$$BOOST_LOCATION/stage/lib

CONFIG(debug, debug|release) {

    DEFINES += TORRENT_DEBUG

    LIBS += -LC:/libtorrent-rasterbar-1.0.2/bin/msvc-11.0/debug/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    # The mailinglist suggested this to be able
    LIBS += DbgHelp.lib

    message("Debug Configuration")
}

CONFIG(release, debug|release) {

    DEFINES += NDEBUG

    LIBS += -LC:/libtorrent-rasterbar-1.0.2/bin/msvc-11.0/release/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    message("Release Configuration")
}
