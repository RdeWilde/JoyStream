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

QT      += core
QT      += sql
QT      += network # network due to blockcypher integration
QT      += testlib

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher/include

# wallet ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../wallet/release/ -lwallet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../wallet/debug/ -lwallet
else:unix: LIBS += -L$$OUT_PWD/../../wallet/ -lwallet

INCLUDEPATH += $$PWD/../../wallet/include
DEPENDPATH += $$PWD/../../wallet/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/release/libwallet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/debug/libwallet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/release/wallet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/debug/wallet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../wallet/libwallet.a

# common ###############################################################
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

include(../../mSIGNA.pri)
include(../../openssl.pri)
include(../../boost.pri)
