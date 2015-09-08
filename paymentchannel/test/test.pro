# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2015

TARGET = wallet-test
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  += testcase

QT      += core sql
QT      += testlib


include(../../mSIGNA.pri)
include(../../openssl.pri)
include(../../boost.pri)

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

INCLUDEPATH += ../lib/include
