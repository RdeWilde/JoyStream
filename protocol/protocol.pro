# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = protocol
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core # !!!!!!!!!remove later when QDataStream is gone!!!!!!!!!!!!!!!!!

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/protocol/wire/ExtendedMessagePayload.hpp \
    include/protocol/wire/SignRefund.hpp \
    include/protocol/wire/Sell.hpp \
    include/protocol/wire/RequestFullPiece.hpp \
    include/protocol/wire/RefundSigned.hpp \
    include/protocol/wire/Ready.hpp \
    include/protocol/wire/Payment.hpp \
    include/protocol/wire/Observe.hpp \
    include/protocol/wire/NoPayloadMessage.hpp \
    include/protocol/wire/MessageType.hpp \
    include/protocol/wire/JoiningContract.hpp \
    include/protocol/wire/JoinContract.hpp \
    include/protocol/wire/FullPiece.hpp \
    include/protocol/wire/Buy.hpp \
    include/protocol/wire/MessageType.hpp \
    include/protocol/Session.hpp \
    include/protocol/Connection.hpp \
    include/protocol/Mode.hpp \
    include/protocol/PeerModeAnnounced.hpp \
    include/protocol/sell/Terms.hpp \
    include/protocol/sell/ClientState.hpp \
    include/protocol/sell/PeerState.hpp \
    include/protocol/sell/Session.hpp \
    include/protocol/sell/Connection.hpp \
    include/protocol/buy/SessionState.hpp \
    include/protocol/buy/Terms.hpp \
    include/protocol/buy/ClientState.hpp \
    include/protocol/buy/PeerState.hpp \
    include/protocol/buy/Piece.hpp \
    include/protocol/buy/Session.hpp \
    include/protocol/buy/Connection.hpp \
    include/protocol/observe/Session.hpp

SOURCES += \
    src/wire/ExtendedMessagePayload.cpp \
    src/wire/SignRefund.cpp \
    src/wire/Sell.cpp \
    src/wire/RequestFullPiece.cpp \
    src/wire/RefundSigned.cpp \
    src/wire/Ready.cpp \
    src/wire/Payment.cpp \
    src/wire/Observe.cpp \
    src/wire/NoPayloadMessage.cpp \
    src/wire/MessageType.cpp \
    src/wire/JoiningContract.cpp \
    src/wire/JoinContract.cpp \
    src/wire/FullPiece.cpp \
    src/wire/Buy.cpp \
    src/PeerModeAnnounced.cpp \
    src/Session.cpp \
    src/Connection.cpp \
    src/sell/Connection.cpp \
    src/buy/Connection.cpp \
    src/buy/Piece.cpp \
    src/buy/Session.cpp \
    src/observe/Session.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
