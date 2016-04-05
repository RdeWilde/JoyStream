# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = wire
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core # !!!!!!!!!remove later when QDataStream is gone!!!!!!!!!!!!!!!!!

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/wire/ExtendedMessagePayload.hpp \
    include/wire/SignRefund.hpp \
    include/wire/Sell.hpp \
    include/wire/RequestFullPiece.hpp \
    include/wire/RefundSigned.hpp \
    include/wire/Ready.hpp \
    include/wire/Payment.hpp \
    include/wire/Observe.hpp \
    include/wire/NoPayloadMessage.hpp \
    include/wire/MessageType.hpp \
    include/wire/JoiningContract.hpp \
    include/wire/JoinContract.hpp \
    include/wire/FullPiece.hpp \
    include/wire/Buy.hpp \
    include/wire/MessageType.hpp \
    include/wire/SellerTerms.hpp \
    include/wire/BuyerTerms.hpp \
    include/wire/ContractInvitation.hpp \
    include/wire/ContractRSVP.hpp \
    include/wire/PieceData.hpp \

SOURCES += \
    src/ExtendedMessagePayload.cpp \
    src/SignRefund.cpp \
    src/Sell.cpp \
    src/RequestFullPiece.cpp \
    src/RefundSigned.cpp \
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
    src/ContractInvitation.cpp \
    src/ContractRSVP.cpp \
    src/PieceData.cpp \

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
