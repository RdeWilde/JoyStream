# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015

include(../../mSIGNA.pri)
include(../../openssl.pri)
include(../../boost.pri)

TARGET = common-test
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  += testcase

QT      += core
QT      += testlib

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

# common ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lcommon

INCLUDEPATH += $$PWD/../lib/include
DEPENDPATH += $$PWD/../lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libcommon.a
