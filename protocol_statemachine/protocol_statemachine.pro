# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = protocol_statemachine
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core # !!!!!!!!!remove later when QDataStream is gone!!!!!!!!!!!!!!!!!

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/protocol_statemachine/ModeAnnounced.hpp \
    include/protocol_statemachine/CBStateMachine.hpp \
    include/protocol_statemachine/ChooseMode.hpp \
    include/protocol_statemachine/AnnouncedModeAndTerms.hpp \
    include/protocol_statemachine/event/ObserveModeStarted.hpp \
    include/protocol_statemachine/event/SellModeStarted.hpp \
    include/protocol_statemachine/event/BuyModeStarted.hpp \
    include/protocol_statemachine/event/Recv.hpp \
    include/protocol_statemachine/event/UpdateTerms.hpp \
    include/protocol_statemachine/event/InviteSeller.hpp \
    include/protocol_statemachine/event/PieceLoaded.hpp \
    include/protocol_statemachine/Active.hpp \
    include/protocol_statemachine/Selling.hpp \
    include/protocol_statemachine/Buying.hpp \
    include/protocol_statemachine/Observing.hpp \
    include/protocol_statemachine/ReadyForInvitation.hpp \
    include/protocol_statemachine/Invited.hpp \
    include/protocol_statemachine/WaitingToStart.hpp \
    include/protocol_statemachine/event/Joined.hpp \
    include/protocol_statemachine/ReadyForPieceRequest.hpp \
    include/protocol_statemachine/ServicingPieceRequest.hpp \
    include/protocol_statemachine/WaitingForPayment.hpp \
    include/protocol_statemachine/detail/InitializeBuying.hpp \
    include/protocol_statemachine/detail/InitializeSelling.hpp \
    include/protocol_statemachine/detail/InitializeObserving.hpp \
    include/protocol_statemachine/exception/StateIncompatibleEvent.hpp \
    include/protocol_statemachine/exception/InvitedToJoinContractByNonBuyer.hpp \
    include/protocol_statemachine/exception/CannotInviteNonSeller.hpp \
    include/protocol_statemachine/WaitingForSellerToJoin.hpp \
    include/protocol_statemachine/SellerHasJoined.hpp \
    include/protocol_statemachine/event/ContractPrepared.hpp \
    include/protocol_statemachine/ReadyToRequestPiece.hpp \
    include/protocol_statemachine/event/RequestPiece.hpp \
    include/protocol_statemachine/LoadingPiece.hpp \
    include/protocol_statemachine/WaitingForFullPiece.hpp \
    include/protocol_statemachine/ProcessingPiece.hpp \
    include/protocol_statemachine/event/SendPayment.hpp \
    include/protocol_statemachine/event/InvalidPieceReceived.hpp \
    include/protocol_statemachine/protocol_statemachine.hpp \
    include/protocol_statemachine/event/event.hpp \
    include/protocol_statemachine/exception/exception.hpp


SOURCES += \
    src/AnnouncedModeAndTerms.cpp \
    src/ServicingPieceRequest.cpp \
    src/CBStateMachine.cpp \
    src/event/SellModeStarted.cpp \
    src/event/BuyModeStarted.cpp \
    src/ChooseMode.cpp \
    src/event/SellModeStarted.cpp \
    src/Active.cpp \
    src/Selling.cpp \
    src/Buying.cpp \
    src/Observing.cpp \
    src/ReadyForInvitation.cpp \
    src/Invited.cpp \
    src/exception/InvitedToJoinContractByNonBuyer.cpp \
    src/exception/StateIncompatibleEvent.cpp \
    src/event/UpdateTerms.cpp \
    src/event/Joined.cpp \
    src/WaitingToStart.cpp \
    src/ReadyForPieceRequest.cpp \
    src/event/PieceLoaded.cpp \
    src/WaitingForPayment.cpp \
    src/detail/InitializeBuying.cpp \
    src/detail/InitializeSelling.cpp \
    src/event/InviteSeller.cpp \
    src/ReadyToInviteSeller.cpp \
    src/exception/CannotInviteNonSeller.cpp \
    src/WaitingForSellerToJoin.cpp \
    src/SellerHasJoined.cpp \
    src/PreparingContract.cpp \
    src/event/ContractPrepared.cpp \
    src/event/RequestPiece.cpp \
    src/ReadyToRequestPiece.cpp \
    src/LoadingPiece.cpp \
    src/WaitingForFullPiece.cpp \
    src/ProcessingPiece.cpp \

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# protocol_wire
INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
