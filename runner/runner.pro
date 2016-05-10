# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(config.pri)

TARGET = runner
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT      += core gui network sql # sql is for wallet

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/runner/Runner.hpp \
    include/runner/ControllerBarrier.hpp

SOURCES += \
    src/Runner.cpp \
    src/ControllerBarrier.cpp

INCLUDEPATH += $$PWD/../core/include
DEPENDPATH += $$PWD/../core/include

INCLUDEPATH += $$PWD/../views/gui/include
DEPENDPATH += $$PWD/../views/gui/include

INCLUDEPATH += $$PWD/../blockcypher/include/
DEPENDPATH += $$PWD/../blockcypher/include/

INCLUDEPATH += $$PWD/../paymentchannel/include/
DEPENDPATH += $$PWD/../paymentchannel/include/

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

INCLUDEPATH += $$PWD/../bitcoin/include
DEPENDPATH += $$PWD/../bitcoin/include

# some of these may be removeable later
include(../config.pri)
