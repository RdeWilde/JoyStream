# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential

QT -= gui

TARGET = bitcoin
TEMPLATE = lib
CONFIG += staticlib
CONFIG  += c++11

INCLUDEPATH += $$PWD/include

INCLUDEPATH += $$PWD/../common/include

#store
HEADERS += include/bitcoin/Store.hpp \
           src/detail/store/Schema.hpp

SOURCES += src/detail/store/Schema-odb.cxx \
           src/detail/store/Schema.cpp \
           src/Store.cpp


include(../config.pri)
