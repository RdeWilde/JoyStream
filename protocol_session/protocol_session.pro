# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016

TARGET = protocol_session
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core # !!!!!!!!!remove later when QDataStream is gone!!!!!!!!!!!!!!!!!

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/protocol_session/detail/Piece.hpp \
    include/protocol_session/detail/Seller.hpp \
    include/protocol_session/detail/Connection.hpp \
    include/protocol_session/detail/Selling.hpp \
    include/protocol_session/detail/Buying.hpp \
    include/protocol_session/protocol_session.hpp \
    include/protocol_session/Status.hpp \
    include/protocol_session/TorrentPieceInformation.hpp \
    include/protocol_session/PieceInformation.hpp \
    include/protocol_session/Exceptions.hpp \
    include/protocol_session/Callbacks.hpp \
    include/protocol_session/Session.hpp \
    include/protocol_session/SessionMode.hpp \
    include/protocol_session/SessionState.hpp \
    include/protocol_session/SellingPolicy.hpp \
    include/protocol_session/BuyingPolicy.hpp \
    include/protocol_session/detail/Observing.hpp \
    include/protocol_session/BuyingState.hpp \
    include/protocol_session/PieceState.hpp \
    include/protocol_session/SellerState.hpp


SOURCES += \
    src/detail/Seller.cpp \
    src/detail/Connection.cpp \
    src/TorrentPieceInformation.cpp \
    src/PieceInformation.cpp \
    src/Session.cpp \
    src/detail/Observing.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# protocol_statemachine
INCLUDEPATH += $$PWD/../protocol_statemachine/include
DEPENDPATH += $$PWD/../protocol_statemachine/include

# protocol_wire
INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

include(../config.pri)
