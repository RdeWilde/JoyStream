# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = wallet
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core sql network # network added due to blockcypher integration

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/protocol/Message/SignRefund.hpp \
    include/protocol/Message/Sell.hpp \
    include/protocol/Message/RequestFullPiece.hpp \
    include/protocol/Message/RefundSigned.hpp \
    include/protocol/Message/Ready.hpp \
    include/protocol/Message/Payment.hpp \
    include/protocol/Message/Observe.hpp \
    include/protocol/Message/NoPayloadMessage.hpp \
    include/protocol/Message/MessageType.hpp \
    include/protocol/Message/JoiningContract.hpp \
    include/protocol/Message/JoinContract.hpp \
    include/protocol/Message/FullPiece.hpp \
    include/protocol/Message/ExtendedMessageTools.hpp \
    include/protocol/Message/ExtendedMessagePayload.hpp \
    include/protocol/Message/ExtendedMessageIdMapping.hpp \
    include/protocol/Message/Buy.hpp

SOURCES += \
    src/Message/SignRefund.cpp \
    src/Message/Sell.cpp \
    src/Message/RequestFullPiece.cpp \
    src/Message/RefundSigned.cpp \
    src/Message/Ready.cpp \
    src/Message/Payment.cpp \
    src/Message/Observe.cpp \
    src/Message/NoPayloadMessage.cpp \
    src/Message/MessageType.cpp \
    src/Message/JoiningContract.cpp \
    src/Message/JoinContract.cpp \
    src/Message/FullPiece.cpp \
    src/Message/ExtendedMessageTools.cpp \
    src/Message/ExtendedMessagePayload.cpp \
    src/Message/ExtendedMessageIdMapping.cpp \
    src/Message/End.cpp \
    src/Message/Buy.cpp


# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
