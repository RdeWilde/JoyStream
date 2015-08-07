# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(../mSIGNA.pri) # need stdutils/uchar_vector.h

TARGET = common
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/common/BitCoinRepresentation.hpp \
    include/common/DataSizeRepresentation.hpp \
    include/common/Fiat.hpp \
    include/common/BitCoinDisplaySettings.hpp \
    include/common/CoinWrappers.hpp

SOURCES += \
    src/BitCoinRepresentation.cpp \
    src/DataSizeRepresentation.cpp \
    src/BitCoinDisplaySettings.cpp \
    src/CoinWrappers.cpp

