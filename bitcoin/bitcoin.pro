# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential

QT -= gui
QT += network # required for blockcypher module

TARGET = bitcoin
TEMPLATE = lib
CONFIG += staticlib
CONFIG  += c++11

INCLUDEPATH += $$PWD/include

INCLUDEPATH += $$PWD/../common/include

INCLUDEPATH += $$PWD/../blockcypher/include

#store
HEADERS += include/bitcoin/Store.hpp \
           src/detail/store/Schema.hpp \
           include/bitcoin/BlockCypherWallet.hpp \
           include/bitcoin/SPVWallet.hpp \
           #include/bitcoin/SPVClient.hpp

SOURCES += src/detail/store/Schema-odb.cxx \
           src/detail/store/Schema.cpp \
           src/Store.cpp \
           src/BlockCypherWallet.cpp \
           src/SPVWallet.cpp \
           #src/SPVClient.cpp

include(../config.pri)
