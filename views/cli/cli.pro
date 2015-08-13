# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

include(../../mSIGNA.pri) # needed for <stdutils/uchar_vector.h>
include(../../libtorrent.pri)
include(../../boost.pri)

TARGET = cli
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core

INCLUDEPATH += $$PWD/include
