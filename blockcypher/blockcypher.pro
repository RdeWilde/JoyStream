# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015

TARGET = blockcypher
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network # used by blockcypher module

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/blockcypher/Wallet.hpp \
    include/blockcypher/TXRef.hpp \
    include/blockcypher/Address.hpp \
    include/blockcypher/Client.hpp \
    include/blockcypher/CreateWallet.hpp \
    include/blockcypher/BlockCypher.hpp

SOURCES += \
    src/Wallet.cpp \
    src/Client.cpp \
    src/CreateWallet.cpp \
    src/BlockCypher.cpp

# common #############################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/lib/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/lib/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/lib/ -lcommon

INCLUDEPATH += $$PWD/../common/lib/include
DEPENDPATH += $$PWD/../common/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/lib/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/lib/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/lib/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/lib/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/lib/libcommon.a


include(../mSIGNA.pri) # needed for stdutils/uchar_vector.h
include(../openssl.pri)
include(../boost.pri)
