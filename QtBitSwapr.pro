#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T10:43:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBitSwapr
TEMPLATE = app

SOURCES +=  main.cpp\
            mainwindow.cpp\
            ../BitSwapr/core/src/ConsoleView.cpp \
            ../BitSwapr/core/src/Controller.cpp \
            ../BitSwapr/core/src/ControllerState.cpp \
            ../BitSwapr/core/src/ViewRequestCallbackHandler.cpp \
            ../BitSwapr/core/src/Exceptions/InvalidBitSwaprStateEntryException.cpp \
            ../BitSwapr/core/src/Exceptions/ListenOnException.cpp \
            ../BitSwapr/core/src/Exceptions/MissingInfoHashViewRequestException.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/boost_1_56_0
INCLUDEPATH += C:/libtorrent-rasterbar-1.0.2/include
INCLUDEPATH += C:/Users/bedeho/Documents/GitHub/BitSwapr/core/include

DEFINES += WIN32
DEFINES += NDEBUG
DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += TORRENT_DISABLE_GEO_IP
DEFINES += TORRENT_NO_DEPRECATE

LIBS += -LC:/boost_1_56_0/stage/lib

debug {
    LIBS += -LC:/libtorrent-rasterbar-1.0.2/bin/msvc-11.0/debug/boost-source/deprecated-functions-off/link-static/threading-multi/ -llibtorrent
} release {
    LIBS += -LC:/libtorrent-rasterbar-1.0.2/bin/msvc-11.0/release/boost-source/deprecated-functions-off/link-static/threading-multi/ -llibtorrent
}
