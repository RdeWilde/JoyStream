# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 23 2015

include(config.pri)

TARGET = extension
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/extension/TorrentPlugin.hpp \
    include/extension/Plugin.hpp \
    include/extension/PeerPlugin.hpp \
    include/extension/PeerPluginId.hpp \
    include/extension/BEPSupportStatus.hpp \
    include/extension/PluginMode.hpp \
    include/extension/Request/PluginRequest.hpp \
    include/extension/Request/TorrentPluginRequest.hpp \
    include/extension/Request/PeerPluginRequest.hpp \
    include/extension/Request/TorrentPluginRequestType.hpp \
    include/extension/Request/PluginRequestType.hpp \
    include/extension/Request/PeerPluginRequestType.hpp \
    include/extension/Request/StartBuyerTorrentPlugin.hpp \
    include/extension/Request/StartSellerTorrentPlugin.hpp \
    include/extension/Request/StartObserverTorrentPlugin.hpp \
    include/extension/Request/ChangeDownloadLocation.hpp \
    include/extension/Message/ExtendedMessageIdMapping.hpp \
    include/extension/Message/MessageType.hpp \
    include/extension/Message/ExtendedMessagePayload.hpp \
    include/extension/Message/JoiningContract.hpp \
    include/extension/Message/SignRefund.hpp \
    include/extension/Message/RefundSigned.hpp \
    include/extension/Message/Sell.hpp \
    include/extension/Message/Observe.hpp \
    include/extension/Message/JoinContract.hpp \
    include/extension/Message/Buy.hpp \
    include/extension/Message/Payment.hpp \
    include/extension/Message/Ready.hpp \
    include/extension/Message/NoPayloadMessage.hpp \
    include/extension/Message/RequestFullPiece.hpp \
    include/extension/Message/FullPiece.hpp \
    include/extension/Message/ExtendedMessageTools.hpp \
    include/extension/PeerAction.hpp \
    include/extension/SellModePeerAction.hpp \
    include/extension/BuyModePeerAction.hpp \
    include/extension/BuyerTorrentPlugin.hpp \
    include/extension/SellerTorrentPlugin.hpp \
    include/extension/BuyerPeerPlugin.hpp \
    include/extension/SellerPeerPlugin.hpp \
    include/extension/Alert/BuyerPeerAddedAlert.hpp \
    include/extension/Alert/SellerPeerAddedAlert.hpp \
    include/extension/Alert/BuyerPeerPluginRemovedAlert.hpp \
    include/extension/Alert/SellerPeerPluginRemovedAlert.hpp \
    include/extension/Alert/TorrentPluginAlert.hpp \
    include/extension/Alert/AlertTypes.hpp \
    include/extension/Alert/PluginStatusAlert.hpp \
    include/extension/Alert/BuyerTorrentPluginStatusAlert.hpp \
    include/extension/Alert/TorrentPluginStartedAlert.hpp \
    include/extension/Alert/SellerTorrentPluginStatusAlert.hpp \
    include/extension/Alert/StartedSellerTorrentPlugin.hpp \
    include/extension/Alert/StartedBuyerTorrentPlugin.hpp \
    include/extension/status/PeerPlugin.hpp \
    include/extension/PeerModeAnnounced.hpp

SOURCES += \
    src/TorrentPlugin.cpp \
    src/Plugin.cpp \
    src/PeerPlugin.cpp \
    src/PeerPluginId.cpp \
    src/Request/PluginRequest.cpp \
    src/Request/TorrentPluginRequest.cpp \
    src/Request/PeerPluginRequest.cpp \
    src/Request/StartBuyerTorrentPlugin.cpp \
    src/Request/StartSellerTorrentPlugin.cpp \
    src/Request/StartObserverTorrentPlugin.cpp \
    src/Request/ChangeDownloadLocation.cpp \
    src/Message/ExtendedMessageIdMapping.cpp \
    src/Message/ExtendedMessagePayload.cpp \
    src/Message/JoiningContract.cpp \
    src/Message/SignRefund.cpp \
    src/Message/RefundSigned.cpp \
    src/Message/Buy.cpp \
    src/Message/Sell.cpp \
    src/Message/Observe.cpp \
    src/Message/JoinContract.cpp \
    src/Message/Payment.cpp \
    src/Message/Ready.cpp \
    src/Message/NoPayloadMessage.cpp \
    src/Message/RequestFullPiece.cpp \
    src/Message/FullPiece.cpp \
    src/Message/ExtendedMessageTools.cpp \
    src/Alert/TorrentPluginAlert.cpp \
    src/Alert/PluginStatusAlert.cpp \
    src/Alert/BuyerTorrentPluginStatusAlert.cpp \
    src/Alert/TorrentPluginStartedAlert.cpp \
    src/Alert/SellerTorrentPluginStatusAlert.cpp \
    src/Alert/StartedSellerTorrentPlugin.cpp \
    src/Alert/StartedBuyerTorrentPlugin.cpp \
    src/Alert/BuyerPeerAddedAlert.cpp \
    src/Alert/SellerPeerAddedAlert.cpp \
    src/Alert/BuyerPeerPluginRemovedAlert.cpp \
    src/Alert/SellerPeerPluginRemovedAlert.cpp \
    src/BuyerTorrentPlugin.cpp \
    src/SellerTorrentPlugin.cpp \
    src/SellerPeerPlugin.cpp \
    src/BuyerPeerPlugin.cpp \
    src/status/PeerPlugin.cpp


# common
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

# paymentchannel
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



include(../config.pri)
