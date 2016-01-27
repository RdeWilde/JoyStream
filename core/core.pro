# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(config.pri)

TARGET = core
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core gui network sql # sql is for wallet

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/core/logger/LoggerManager.hpp \
    include/core/logger/exceptions/DuplicateLog.hpp \
    include/core/logger/exceptions/CannnotOpenLogFile.hpp \
    include/core/viewmodels/BuyerPeerPluginViewModel.hpp \
    include/core/viewmodels/BuyerTorrentPluginViewModel.hpp \
    include/core/viewmodels/ChannelViewModel.hpp \
    include/core/viewmodels/PayeeViewModel.hpp \
    include/core/viewmodels/PayorViewModel.hpp \
    include/core/viewmodels/PeerPluginViewModel.hpp \
    include/core/viewmodels/SellerPeerPluginViewModel.hpp \
    include/core/viewmodels/SellerTorrentPluginViewModel.hpp \
    include/core/viewmodels/TorrentPluginViewModel.hpp \
    include/core/viewmodels/TorrentViewModel.hpp \
    include/core/Controller.hpp \
    include/core/PluginInstalled.hpp \
    include/core/Stream.hpp \
    include/core/configurations/Torrent.hpp \
    include/core/configurations/Controller.hpp

SOURCES += \
    src/logger/LoggerManager.cpp \
    src/logger/exceptions/CannnotOpenLogFile.cpp \
    src/logger/exceptions/DuplicateLog.cpp \
    src/viewmodels/BuyerPeerPluginViewModel.cpp \
    src/viewmodels/BuyerTorrentPluginViewModel.cpp \
    src/viewmodels/ChannelViewModel.cpp \
    src/viewmodels/PayeeViewModel.cpp \
    src/viewmodels/PayorViewModel.cpp \
    src/viewmodels/PeerPluginViewModel.cpp \
    src/viewmodels/SellerPeerPluginViewModel.cpp \
    src/viewmodels/SellerTorrentPluginViewModel.cpp \
    src/viewmodels/TorrentPluginViewModel.cpp \
    src/viewmodels/TorrentViewModel.cpp \
    src/Controller.cpp \
    src/Stream.cpp \
    src/configurations/Torrent.cpp \
    src/configurations/Controller.cpp

# paymentchannel ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../paymentchannel/ -lpaymentchannel

INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/libpaymentchannel.a

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

# wallet ###############################################################
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

# common ###############################################################
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

include(../config.pri)
