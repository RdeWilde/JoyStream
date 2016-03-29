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
    include/protocol/ContractInvitation.hpp \
    include/protocol/ModeAnnounced.hpp \
    include/protocol/ContractRSVP.hpp \
    include/protocol/statemachine/CBStateMachine.hpp \
    include/protocol/statemachine/ChooseMode.hpp \
    include/protocol/statemachine/event/ObserveModeStarted.hpp \
    include/protocol/statemachine/event/SellModeStarted.hpp \
    include/protocol/statemachine/event/BuyModeStarted.hpp \
    include/protocol/statemachine/event/Recv.hpp \
    include/protocol/statemachine/event/UpdateTerms.hpp \
    include/protocol/statemachine/event/InviteSeller.hpp \
    include/protocol/statemachine/event/PieceLoaded.hpp \
    include/protocol/statemachine/Active.hpp \
    include/protocol/statemachine/Selling.hpp \
    include/protocol/statemachine/Buying.hpp \
    include/protocol/statemachine/Observing.hpp \
    include/protocol/statemachine/ReadyForInvitation.hpp \
    include/protocol/statemachine/Invited.hpp \
    include/protocol/statemachine/WaitingToStart.hpp \
    include/protocol/statemachine/event/Joined.hpp \
    include/protocol/statemachine/ReadyForPieceRequest.hpp \
    include/protocol/statemachine/ServicingPieceRequest.hpp \
    include/protocol/statemachine/WaitingForPayment.hpp \
    include/protocol/statemachine/detail/InitializeBuying.hpp \
    include/protocol/statemachine/detail/InitializeSelling.hpp \
    include/protocol/statemachine/detail/InitializeObserving.hpp \
    include/protocol/statemachine/exception/StateIncompatibleEvent.hpp \
    include/protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp \
    include/protocol/statemachine/exception/CannotInviteNonSeller.hpp \
    include/protocol/statemachine/WaitingForSellerToJoin.hpp \
    include/protocol/statemachine/SellerHasJoined.hpp \
    include/protocol/statemachine/event/ContractPrepared.hpp \
    include/protocol/statemachine/ReadyToRequestPiece.hpp \
    include/protocol/statemachine/event/RequestPiece.hpp \
    include/protocol/statemachine/LoadingPiece.hpp \
    include/protocol/statemachine/WaitingForFullPiece.hpp \
    include/protocol/statemachine/ProcessingPiece.hpp \
    include/protocol/PieceData.hpp \
    include/protocol/statemachine/event/SendPayment.hpp \
    include/protocol/statemachine/event/InvalidPieceReceived.hpp

SOURCES += \
    src/wire/ExtendedMessagePayload.cpp \
    src/wire/SignRefund.cpp \
    src/wire/Sell.cpp \
    src/wire/RequestFullPiece.cpp \
    src/wire/RefundSigned.cpp \
    src/wire/Ready.cpp \
    src/wire/Payment.cpp \
    src/wire/NoPayloadMessage.cpp \
    src/wire/MessageType.cpp \
    src/wire/JoiningContract.cpp \
    src/wire/JoinContract.cpp \
    src/wire/FullPiece.cpp \
    src/wire/Buy.cpp \
    src/wire/Observe.cpp \
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
    src/ContractInvitation.cpp \
    src/ContractRSVP.cpp \
    src/statemachine/ServicingPieceRequest.cpp \
    src/statemachine/CBStateMachine.cpp \
    src/statemachine/event/SellModeStarted.cpp \
    src/statemachine/event/BuyModeStarted.cpp \
    src/statemachine/ChooseMode.cpp \
    src/statemachine/event/SellModeStarted.cpp \
    src/statemachine/Active.cpp \
    src/statemachine/Selling.cpp \
    src/statemachine/Buying.cpp \
    src/statemachine/Observing.cpp \
    src/statemachine/ReadyForInvitation.cpp \
    src/statemachine/Invited.cpp \
    src/statemachine/exception/InvitedToJoinContractByNonBuyer.cpp \
    src/statemachine/exception/StateIncompatibleEvent.cpp \
    src/statemachine/event/UpdateTerms.cpp \
    src/statemachine/event/Joined.cpp \
    src/statemachine/WaitingToStart.cpp \
    src/statemachine/ReadyForPieceRequest.cpp \
    src/statemachine/event/PieceLoaded.cpp \
    src/statemachine/WaitingForPayment.cpp \
    src/statemachine/detail/InitializeBuying.cpp \
    src/statemachine/detail/InitializeSelling.cpp \
    src/statemachine/event/InviteSeller.cpp \
    src/statemachine/ReadyToInviteSeller.cpp \
    src/statemachine/exception/CannotInviteNonSeller.cpp \
    src/statemachine/WaitingForSellerToJoin.cpp \
    src/statemachine/SellerHasJoined.cpp \
    src/statemachine/PreparingContract.cpp \
    src/statemachine/event/ContractPrepared.cpp \
    src/statemachine/event/RequestPiece.cpp \
    src/statemachine/ReadyToRequestPiece.cpp \
    src/statemachine/LoadingPiece.cpp \
    src/statemachine/WaitingForFullPiece.cpp \
    src/PieceData.cpp \
    src/statemachine/ProcessingPiece.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
