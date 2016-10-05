# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016

# not needed quite yet: include(config.pri)

TARGET = controller
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

HEADERS += \
    include/controller/ApplicationController.hpp \
    include/controller/Torrent.hpp \
    include/controller/Peer.hpp \
    include/controller/PeerPlugin.hpp \
    include/controller/TorrentPlugin.hpp \
    include/controller/Session.hpp \
    include/controller/Connection.hpp \
    include/controller/Payor.hpp \
    include/controller/Payee.hpp \
    include/controller/CBStateMachine.hpp \
    include/controller/Selling.hpp \
    include/controller/Seller.hpp \
    include/controller/Buying.hpp \

SOURCES += \
    src/ApplicationController.cpp \
    src/Torrent.cpp \
    src/Peer.cpp \
    src/TorrentPlugin.cpp \
    src/PeerPlugin.cpp \
    src/Session.cpp \
    src/Payor.cpp \
    src/Payee.cpp \
    src/CBStateMachine.cpp \
    src/Connection.cpp \
    src/Selling.cpp \
    src/Seller.cpp \
    src/Buying.cpp \

# gui
INCLUDEPATH += $$PWD/../gui/include
DEPENDPATH += $$PWD/../gui/include

# core
INCLUDEPATH += $$PWD/../../core/include
DEPENDPATH += $$PWD/../../core/include

# extension
INCLUDEPATH += $$PWD/../../extension/include
DEPENDPATH += $$PWD/../../extension/include

# protocol_session
INCLUDEPATH += $$PWD/../../protocol_session/include
DEPENDPATH += $$PWD/../../protocol_session/include

# protocol_statemachine
INCLUDEPATH += $$PWD/../../protocol_statemachine/include
DEPENDPATH += $$PWD/../../protocol_statemachine/include

# protocol_wire
INCLUDEPATH += $$PWD/../../protocol_wire/include
DEPENDPATH += $$PWD/../../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../../paymentchannel/include
DEPENDPATH += $$PWD/../../paymentchannel/include

# common
INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include

include(../../config.pri)
