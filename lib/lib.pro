
# Qt Project
TARGET = QtBitSwapr
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += console

QT     += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

SOURCES += \
            controller/Controller.cpp \
            controller/ControllerState.cpp \
            controller/exceptions/InvalidBitSwaprStateEntryException.cpp \
            controller/exceptions/ListenOnException.cpp \
            controller/exceptions/MissingInfoHashViewRequestException.cpp \
            view/mainwindow.cpp \
            view/addtorrentdialog.cpp \
            extension/BitSwaprPlugin.cpp \
            extension/BitSwaprTorrentPlugin.cpp \
            extension/BitSwaprPeerPlugin.cpp \
            BitSwapr.cpp \
            logger/LoggerManager.cpp \
            logger/exceptions/CannnotOpenLogFile.cpp \
            logger/exceptions/DuplicateLog.cpp
		
HEADERS += \
            controller/Controller.hpp \
            controller/ControllerState.hpp \
            controller/exceptions/InvalidBitSwaprStateEntryException.hpp \
            controller/exceptions/ListenOnException.hpp \
            controller/exceptions/MissingInfoHashViewRequestException.hpp \
            view/mainwindow.hpp \
            view/addtorrentdialog.hpp \
            extension/BitSwaprPlugin.hpp \
            extension/BitSwaprTorrentPlugin.hpp \
            extension/BitSwaprPeerPlugin.hpp \
            Config.hpp \
            BitSwapr.hpp \
            logger/LoggerManager.hpp \
            logger/exceptions/DuplicateLog.hpp \
            logger/exceptions/CannnotOpenLogFile.hpp
				
FORMS += \
            view/mainwindow.ui \
            view/addtorrentdialog.ui

OTHER_FILES += \
            resources/BitSwapr_mark_32.png


# Required for including libtorrent and boost headers
include(../defaults.pri)

# Linking with Libtorrent
CONFIG(debug, debug|release) {

    DEFINES += TORRENT_DEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-11.0/debug/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    # The mailinglist suggested this to be able
    LIBS += DbgHelp.lib

    message("Debug Configuration")
}

CONFIG(release, debug|release) {

    DEFINES += NDEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-11.0/release/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    message("Release Configuration")
}

# Linking with boost
LIBS += -L$$BOOST_LOCATION/stage/lib
