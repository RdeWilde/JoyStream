# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, November 15 2015

TARGET = mixpanel-test
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  += testcase

QT      += core network
QT      += testlib

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

# Mixpanel
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../mixpanel/release/ -lmixpanel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../mixpanel/debug/ -lmixpanel
else:unix: LIBS += -L$$OUT_PWD/../../mixpanel/ -lmixpanel

INCLUDEPATH += $$PWD/../../mixpanel/include
DEPENDPATH += $$PWD/../../mixpanel/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../mixpanel/release/libmixpanel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../mixpanel/debug/libmixpanel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../mixpanel/release/mixpanel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../mixpanel/debug/mixpanel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../mixpanel/libmixpanel.a

include(../../config.pri)
