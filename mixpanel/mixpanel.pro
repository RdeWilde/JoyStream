# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, November 14 2015

TARGET = mixpanel
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/Mixpanel/Event.hpp \
    include/Mixpanel/Mixpanel.hpp

SOURCES += \
    src/Event.cpp \
    src/Mixpanel.cpp

include(../config.pri)
