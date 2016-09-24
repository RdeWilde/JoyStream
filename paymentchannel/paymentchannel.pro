# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015

TARGET = paymentchannel
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core sql

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/paymentchannel/RedeemScript.hpp \
    include/paymentchannel/Commitment.hpp \
    include/paymentchannel/Payee.hpp \
    include/paymentchannel/Refund.hpp \
    include/paymentchannel/Settlement.hpp \
    include/paymentchannel/Termination.hpp \
    include/paymentchannel/Payor.hpp \
    include/paymentchannel/paymentchannel.hpp \
    include/paymentchannel/Contract.hpp

SOURCES += \
    src/RedeemScript.cpp \
    src/Commitment.cpp \
    src/Payee.cpp \
    src/Refund.cpp \
    src/Settlement.cpp \
    src/Termination.cpp \
    src/Payor.cpp \
    src/Contract.cpp

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include


include(../config.pri)
