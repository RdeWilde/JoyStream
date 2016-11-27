TARGET = libtorrent_interface
TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl
CONFIG += c++11

QT += core
QT += network

INCLUDEPATH += $$PWD/../../extension/include
INCLUDEPATH += $$PWD/include

message($$INCLUDEPATH)

HEADERS += \
    include/PeerInterface.hpp \
    include/SessionInterface.hpp \
    include/TorrentInterface.hpp \
    include/Peer.hpp \
    include/Session.hpp \
    include/Torrent.hpp \
    include/AlertManager.hpp \

SOURCES += \
    src/Peer.cpp \
    src/Session.cpp \
    src/Torrent.cpp \
    src/AlertManager.cpp \

include(../../config.pri)
