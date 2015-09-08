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

# common ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/lib/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/lib/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/lib/ -lcommon

INCLUDEPATH += $$PWD/../../common/lib/include
DEPENDPATH += $$PWD/../../common/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/libcommon.a

# paymentchannels ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lpaymentchannel

INCLUDEPATH += $$PWD/../lib/include
DEPENDPATH += $$PWD/../lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libpaymentchannel.a
