#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T10:43:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBitSwapr
TEMPLATE = app

SOURCES +=  src/main.cpp \
    src/controller/src/Exceptions/InvalidBitSwaprStateEntryException.cpp \
    src/controller/src/Exceptions/ListenOnException.cpp \
    src/controller/src/Exceptions/MissingInfoHashViewRequestException.cpp \
    src/controller/src/BrPayment.cpp \
    src/controller/src/Controller.cpp \
    src/controller/src/ControllerState.cpp \
    src/view/src/mainwindow.cpp \
    src/view/src/addtorrentdialog.cpp

HEADERS += \
    src/Config.hpp \
    src/controller/include/Exceptions/InvalidBitSwaprStateEntryException.hpp \
    src/controller/include/Exceptions/ListenOnException.hpp \
    src/controller/include/Exceptions/MissingInfoHashViewRequestException.hpp \
    src/controller/include/BrPayment.hpp \
    src/controller/include/Controller.hpp \
    src/controller/include/ControllerState.hpp \
    src/view/include/mainwindow.h \
    src/view/include/addtorrentdialog.h

FORMS += \
    src/view/ui/mainwindow.ui \
    src/view/ui/addtorrentdialog.ui

INCLUDEPATH += C:/boost_1_56_0
INCLUDEPATH += C:/libtorrent-rasterbar-1.0.2/include
INCLUDEPATH += C:/Users/bedeho/Documents/GitHub/QtBitSwapr/src

DEFINES += WIN32
DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE

LIBS += -LC:/boost_1_56_0/stage/lib

#message($$CONFIG)

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

OTHER_FILES += \
    resources/frostwire-5.7.6.dmg.torrent

