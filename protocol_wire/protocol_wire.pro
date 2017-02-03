# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = protocol_wire
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array
CONFIG  -= core

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/protocol_wire/ExtendedMessagePayload.hpp \
    include/protocol_wire/Sell.hpp \
    include/protocol_wire/RequestFullPiece.hpp \
    include/protocol_wire/Ready.hpp \
    include/protocol_wire/Payment.hpp \
    include/protocol_wire/Observe.hpp \
    include/protocol_wire/NoPayloadMessage.hpp \
    include/protocol_wire/MessageType.hpp \
    include/protocol_wire/JoiningContract.hpp \
    include/protocol_wire/JoinContract.hpp \
    include/protocol_wire/FullPiece.hpp \
    include/protocol_wire/Buy.hpp \
    include/protocol_wire/MessageType.hpp \
    include/protocol_wire/SellerTerms.hpp \
    include/protocol_wire/BuyerTerms.hpp \
    include/protocol_wire/PieceData.hpp \
    include/protocol_wire/protocol_wire.hpp

SOURCES += \
    src/ExtendedMessagePayload.cpp \
    src/Sell.cpp \
    src/RequestFullPiece.cpp \
    src/Ready.cpp \
    src/Payment.cpp \
    src/NoPayloadMessage.cpp \
    src/MessageType.cpp \
    src/JoiningContract.cpp \
    src/JoinContract.cpp \
    src/FullPiece.cpp \
    src/Buy.cpp \
    src/Observe.cpp \
    src/SellerTerms.cpp \
    src/BuyerTerms.cpp \
    src/PieceData.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
