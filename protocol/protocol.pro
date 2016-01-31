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
    include/protocol/ExtendedMessagePayload.hpp \
    include/protocol/SignRefund.hpp \
    include/protocol/Sell.hpp \
    include/protocol/RequestFullPiece.hpp \
    include/protocol/RefundSigned.hpp \
    include/protocol/Ready.hpp \
    include/protocol/Payment.hpp \
    include/protocol/Observe.hpp \
    include/protocol/NoPayloadMessage.hpp \
    include/protocol/MessageType.hpp \
    include/protocol/JoiningContract.hpp \
    include/protocol/JoinContract.hpp \
    include/protocol/FullPiece.hpp \
    include/protocol/Buy.hpp \
    include/protocol/MessageType.hpp

SOURCES += \
    src/ExtendedMessagePayload.cpp \
    src/SignRefund.cpp \
    src/Sell.cpp \
    src/RequestFullPiece.cpp \
    src/RefundSigned.cpp \
    src/Ready.cpp \
    src/Payment.cpp \
    src/Observe.cpp \
    src/NoPayloadMessage.cpp \
    src/MessageType.cpp \
    src/JoiningContract.cpp \
    src/JoinContract.cpp \
    src/FullPiece.cpp \
    src/Buy.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
