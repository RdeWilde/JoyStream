# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

ICON = joystream.icns
RC_ICONS = joystream.ico

QMAKE_TARGET_COMPANY = JoyStream Inc.
QMAKE_TARGET_PRODUCT = JoyStream
QMAKE_TARGET_DESCRIPTION = A new BitTorrent client, with faster speeds, streaming and paid seeding.
QMAKE_TARGET_COPYRIGHT = JoyStream (c) 2015

include(config.pri)

TARGET = app
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

SOURCES += main.cpp \
    AutoUpdater.cpp \
    Analytics.cpp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

QT      += core network sql # sql needed for wallet

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

# paymentchannel ###############################################################
INCLUDEPATH += $$PWD/../paymentchannel/include

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
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../wallet/release/ -lwallet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../wallet/debug/ -lwallet
else:unix: LIBS += -L$$OUT_PWD/../wallet/ -lwallet

INCLUDEPATH += $$PWD/../wallet/include
DEPENDPATH += $$PWD/../wallet/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/release/libwallet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/debug/libwallet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/release/wallet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../wallet/debug/wallet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../wallet/libwallet.a

# common ###########################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a

include(../mSIGNA.pri) # needed for stdutils/uchar_vector.h
include(../libtorrent.pri)
include(../openssl.pri)
include(../boost.pri)

HEADERS += \
    AutoUpdater.hpp \
    Analytics.hpp

RESOURCES += \
    icon.qrc

