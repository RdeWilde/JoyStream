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
    include/protocol/Mode.hpp \
    include/protocol/PeerModeAnnounced.hpp \
    include/protocol/Piece.hpp \
    include/protocol/BuyerSessionState.hpp \
    include/protocol/Seller.hpp \
    include/protocol/Utilities.hpp \
    include/protocol/ModeAnnounced.hpp \
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
    include/protocol/statemachine/event/SendPayment.hpp \
    include/protocol/statemachine/event/InvalidPieceReceived.hpp \
    include/protocol/Session.hpp \
    include/protocol/Connection.hpp \
    include/protocol/SessionMode.hpp

SOURCES += \
    src/PeerModeAnnounced.cpp \
    src/Piece.cpp \
    src/Seller.cpp \
    src/Utilities.cpp \
    src/Session.cpp \
    src/Connection.cpp \
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
    src/statemachine/ProcessingPiece.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# wire
INCLUDEPATH += $$PWD/../wire/include
DEPENDPATH += $$PWD/../wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
