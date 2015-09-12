# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(../mSIGNA.pri) # needed for stdutils/uchar_vector.h
include(../libtorrent.pri)
include(../openssl.pri)
include(../boost.pri)

TARGET = app
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

SOURCES += main.cpp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

QT      += core network sql # sql needed for wallet

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

# paymentchannel ###############################################################
INCLUDEPATH += $$PWD/../paymentchannel/lib/include

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

# gui ###########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../views/gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../views/gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../views/gui/ -lgui

INCLUDEPATH += $$PWD/../views/gui/include
DEPENDPATH += $$PWD/../views/gui/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../views/gui/release/libgui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../views/gui/debug/libgui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../views/gui/release/gui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../views/gui/debug/gui.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../views/gui/libgui.a

# core ###########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core/include
DEPENDPATH += $$PWD/../core/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a

# wallet ###########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../wallet/lib/release/ -lwallet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../wallet/lib/debug/ -lwallet
else:unix: LIBS += -L$$OUT_PWD/../wallet/lib/ -lwallet

INCLUDEPATH += $$PWD/../wallet/lib/include
DEPENDPATH += $$PWD/../wallet/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/lib/release/libwallet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/lib/debug/libwallet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/lib/release/wallet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/lib/debug/wallet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../wallet/lib/libwallet.a

# common ###########################################################################
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
