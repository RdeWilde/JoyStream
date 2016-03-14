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
    include/protocol/Piece.hpp \
    include/protocol/SellerSession.hpp \
    include/protocol/SellerConnection.hpp \
    include/protocol/SellerTerms.hpp \
    include/protocol/SellerClientState.hpp \
    include/protocol/BuyerSession.hpp \
    include/protocol/BuyerSessionState.hpp \
    include/protocol/BuyerConnection.hpp \
    include/protocol/BuyerTerms.hpp \
    include/protocol/BuyerClientState.hpp \
    include/protocol/ObserverSession.hpp \
    include/protocol/Seller.hpp \
    include/protocol/Utilities.hpp \
    include/protocol/BuyerPayorSettings.hpp \
    include/protocol/BuyerChannelSettings.hpp \
    include/protocol/statemachine/ServicingPieceRequest.hpp \
    include/protocol/statemachine/CBStateMachine.hpp \
    include/protocol/statemachine/SellerJoined.hpp \
    include/protocol/statemachine/ChooseMode.hpp \
    include/protocol/statemachine/event/ObserveModeStarted.hpp \
    include/protocol/statemachine/event/SellModeStarted.hpp \
    include/protocol/statemachine/Observe.hpp \
    include/protocol/statemachine/event/BuyModeStarted.hpp \
    include/protocol/statemachine/event/Recv.hpp \
    include/protocol/statemachine/Active.hpp \
    include/protocol/statemachine/Selling.hpp \
    include/protocol/statemachine/Buying.hpp

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
    src/Connection.cpp \
    src/SellerSession.cpp \
    src/SellerConnection.cpp \
    src/BuyerSession.cpp \
    src/BuyerConnection.cpp \
    src/ObserverSession.cpp \
    src/Piece.cpp \
    src/SellerTerms.cpp \
    src/BuyerTerms.cpp \
    src/Seller.cpp \
    src/Utilities.cpp \
    src/BuyerChannelSettings.cpp \
    src/Session.cpp \
    src/statemachine/ServicingPieceRequest.cpp \
    src/statemachine/SellerJoined.cpp \
    src/statemachine/CBStateMachine.cpp \
    src/statemachine/event/SellModeStarted.cpp \
    src/statemachine/event/BuyModeStarted.cpp \
    src/statemachine/ChooseMode.cpp \
    src/statemachine/Observe.cpp \
    src/statemachine/event/SellModeStarted.cpp \
    src/statemachine/Active.cpp \
    src/statemachine/Selling.cpp \
    src/statemachine/Buying.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
