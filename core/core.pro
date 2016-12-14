# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(config.pri)

TARGET = core
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network # sockets, qnam
QT      += websockets

INCLUDEPATH += $$PWD/include

HEADERS += \
    #include/core/Stream.hpp \
    include/core/core.hpp \
    include/core/Exception.hpp \
    include/core/Torrent.hpp \
    include/core/Node.hpp \
    include/core/detail/detail.hpp \
    include/core/TorrentPlugin.hpp \
    include/core/Peer.hpp \
    #include/core/Session.hpp \
    include/core/PeerPlugin.hpp \
    #include/core/Connection.hpp \
    include/core/MagnetLink.hpp \
    #include/core/Selling.hpp \
    #include/core/Buying.hpp \
    #include/core/Seller.hpp \
    include/core/TorrentIdentifier.hpp \
    #include/core/CBStateMachine.hpp \
    #include/core/Payor.hpp \
    #include/core/Payee.hpp \
    include/core/detail/NodeImpl.hpp \
    include/core/Q_DECLARE_METATYPE.hpp

SOURCES += \
    #src/Stream.cpp \
    src/Torrent.cpp \
    src/Node.cpp \
    src/detail/detail.cpp \
    src/TorrentPlugin.cpp \
    src/MagnetLink.cpp \
    src/Peer.cpp \
    #src/Session.cpp \
    src/PeerPlugin.cpp \
    #src/Selling.cpp \
    #src/Buying.cpp \
    #src/Seller.cpp \
    #src/Connection.cpp \
    src/TorrentIdentifier.cpp \
    #src/Payor.cpp \
    #src/Payee.cpp \
    #src/CBStateMachine.cpp \
    src/detail/NodeImpl.cpp

# extension
INCLUDEPATH += $$PWD/../extension/include
DEPENDPATH += $$PWD/../extension/include

# protocol_session
INCLUDEPATH += $$PWD/../protocol_session/include
DEPENDPATH += $$PWD/../protocol_session/include

# protocol_statemachine
INCLUDEPATH += $$PWD/../protocol_statemachine/include
DEPENDPATH += $$PWD/../protocol_statemachine/include

# protocol_wire
INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
