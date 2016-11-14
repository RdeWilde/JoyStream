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
