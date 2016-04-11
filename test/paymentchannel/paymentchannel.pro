# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2015

TARGET = paymentchannel-test
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  += testcase

QT      += core sql
QT      += testlib

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

# paymentchannels ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../paymentchannel/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../paymentchannel/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../../paymentchannel/ -lpaymentchannel

INCLUDEPATH += $$PWD/../../paymentchannel/include
DEPENDPATH += $$PWD/../../paymentchannel

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/libpaymentchannel.a

# common ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a


include(../../config.pri)
