# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, Septembe 26 2015

include(config.pri)

TARGET = public-one-symmetric
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

QT      += core
QT      += sql
QT      += network websockets # network due to blockcypher integration

SOURCES += \
    main.cpp

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

# runner ########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../runner/release/ -lrunner
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../runner/debug/ -lrunner
else:unix: LIBS += -L$$OUT_PWD/../../runner/ -lrunner

INCLUDEPATH += $$PWD/../../runner/include
DEPENDPATH += $$PWD/../../runner/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../runner/release/librunner.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../runner/debug/librunner.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../runner/release/runner.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../runner/debug/runner.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../runner/librunner.a

# gui ###########################################################################
# remove in the future, we should not require ui for testing really

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../views/gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../views/gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../../views/gui/ -lgui

INCLUDEPATH += $$PWD/../../views/gui/include
DEPENDPATH += $$PWD/../../views/gui/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../views/gui/release/libgui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../views/gui/debug/libgui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../views/gui/release/gui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../views/gui/debug/gui.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../views/gui/libgui.a

# core ###########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lcore

INCLUDEPATH += $$PWD/../../core/include
DEPENDPATH += $$PWD/../../core/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../core/libcore.a

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher/include

# paymentchannel ###############################################################
INCLUDEPATH += $$PWD/../../paymentchannel/include

# common ###########################################################################
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bitcoin/release/ -lbitcoin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bitcoin/debug/ -lbitcoin
else:unix: LIBS += -L$$OUT_PWD/../../bitcoin/ -lbitcoin

INCLUDEPATH += $$PWD/../../bitcoin/include
DEPENDPATH += $$PWD/../../bitcoin/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../bitcoin/release/libbitcoin.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../bitcoin/debug/libbitcoin.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../bitcoin/release/bitcoin.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../bitcoin/debug/bitcoin.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../bitcoin/libbitcoin.a

include(../../config.pri)
