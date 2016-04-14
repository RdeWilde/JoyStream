# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, April 14 2016

TARGET = protocol_wire-test
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

# protocol_wire ###################################################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../protocol_wire/release/ -lprotocol_wire
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../protocol_wire/debug/ -lprotocol_wire
else:unix: LIBS += -L$$OUT_PWD/../../protocol_wire/ -lprotocol_wire

INCLUDEPATH += $$PWD/../../protocol_wire/include
DEPENDPATH += $$PWD/../../protocol_wire/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/release/libprotocol_wire.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/debug/libprotocol_wire.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/release/protocol_wire.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/debug/protocol_wire.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/libprotocol_wire.a

# common ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a

include(../../config.pri)
