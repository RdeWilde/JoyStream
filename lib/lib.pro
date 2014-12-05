
# Qt Project
TARGET = QtBitSwapr
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += console
CONFIG  += c++11 # Needed for class enum

QT     += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

SOURCES += \
    controller/Controller.cpp \
    controller/exceptions/InvalidBitSwaprStateEntryException.cpp \
    controller/exceptions/ListenOnException.cpp \
    controller/exceptions/MissingInfoHashViewRequestException.cpp \
    logger/LoggerManager.cpp \
    logger/exceptions/CannnotOpenLogFile.cpp \
    logger/exceptions/DuplicateLog.cpp \
    view/MainWindow.cpp \
    ControllerTracker.cpp \
    extension/Message/ExtendedMessage.cpp \
    view/AddTorrentDialog.cpp \
    extension/TorrentPluginStatus.cpp \
    extension/TorrentPlugin.cpp \
    extension/Plugin.cpp \
    extension/PeerPlugin.cpp \
    extension/PeerPluginStatus.cpp \
    extension/Message/BuyMessage.cpp \
    extension/Message/SellMessage.cpp \
    extension/Message/ExtendedMessageIdMapping.cpp \
    extension/SellerPeerPlugin.cpp \
    extension/BuyerPeerplugin.cpp \
    view/TorrentViewModel.cpp \
    Utilities.cpp \
    extension/PeerPluginId.cpp \
    view/PeerPluginViewModel.cpp \
    controller/PersistentControllerState.cpp \
    controller/PersistentTorrentState.cpp
		
HEADERS += \
    controller/Controller.hpp \
    controller/exceptions/InvalidBitSwaprStateEntryException.hpp \
    controller/exceptions/ListenOnException.hpp \
    controller/exceptions/MissingInfoHashViewRequestException.hpp \
    Config.hpp \
    logger/LoggerManager.hpp \
    logger/exceptions/DuplicateLog.hpp \
    logger/exceptions/CannnotOpenLogFile.hpp \
    view/MainWindow.hpp \
    ControllerTracker.hpp \
    extension/Message/ExtendedMessage.hpp \
    view/AddTorrentDialog.hpp \
    extension/TorrentPluginStatus.hpp \
    extension/TorrentPlugin.hpp \
    extension/Plugin.hpp \
    extension/PeerPlugin.hpp \
    extension/PeerPluginStatus.hpp \
    extension/Message/BuyMessage.hpp \
    extension/Message/SellMessage.hpp \
    extension/Message/ExtendedMessageIdMapping.hpp \
    extension/SellerPeerPlugin.hpp \
    extension/BuyerPeerPlugin.hpp \
    extension/PeerPluginState.hpp \
    view/TorrentViewModel.hpp \
    Utilities.hpp \
    extension/PeerPluginId.hpp \
    view/PeerPluginViewModel.hpp \
    extension/Message/MessageType.hpp \
    controller/PersistentTorrentState.hpp \
    controller/PersistentControllerState.hpp
				
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
