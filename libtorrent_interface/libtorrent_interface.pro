TARGET = libtorrent_interface
TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl
CONFIG += c++11

QT += core
QT += network

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/PeerInterface.hpp \
    include/SessionInterface.hpp \
    include/TorrentInterface.hpp \
    include/Peer.hpp \
    include/Session.hpp \
    include/Torrent.hpp \

SOURCES += \
    src/Peer.cpp \
    src/Session.cpp \
    src/Torrent.cpp \
