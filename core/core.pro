# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

TARGET = core
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core gui network sql # sql is for wallet

INCLUDEPATH += $$PWD/include

SOURCES += \
    src/controller/Controller.cpp \
    src/controller/ControllerTracker.cpp \
    src/controller/exceptions/InvalidBitSwaprStateEntryException.cpp \
    src/controller/exceptions/ListenOnException.cpp \
    src/controller/exceptions/MissingInfoHashViewRequestException.cpp \
    src/logger/LoggerManager.cpp \
    src/logger/exceptions/CannnotOpenLogFile.cpp \
    src/logger/exceptions/DuplicateLog.cpp \
    src/extension/TorrentPlugin.cpp \
    src/extension/Plugin.cpp \
    src/extension/PeerPlugin.cpp \
    src/extension/PeerPluginId.cpp \
    src/extension/Request/PluginRequest.cpp \
    src/extension/Request/TorrentPluginRequest.cpp \
    src/extension/Request/PeerPluginRequest.cpp \
    src/extension/Request/StartBuyerTorrentPlugin.cpp \
    src/extension/Request/StartSellerTorrentPlugin.cpp \
    src/extension/Request/StartObserverTorrentPlugin.cpp \
    src/extension/Request/ChangeDownloadLocation.cpp \
    src/extension/Message/ExtendedMessageIdMapping.cpp \
    src/extension/Message/ExtendedMessagePayload.cpp \
    src/extension/Message/JoiningContract.cpp \
    src/extension/Message/SignRefund.cpp \
    src/extension/Message/RefundSigned.cpp \
    src/extension/Message/Buy.cpp \
    src/extension/Message/Sell.cpp \
    src/extension/Message/Observe.cpp \
    src/extension/Message/JoinContract.cpp \
    src/extension/Message/Payment.cpp \
    src/extension/Message/Ready.cpp \
    src/extension/Message/NoPayloadMessage.cpp \
    src/extension/Message/RequestFullPiece.cpp \
    src/extension/Message/FullPiece.cpp \
    src/extension/Message/ExtendedMessageTools.cpp \
    src/extension/Alert/TorrentPluginAlert.cpp \
    src/extension/Alert/PluginStatusAlert.cpp \
    src/extension/Alert/BuyerTorrentPluginStatusAlert.cpp \
    src/extension/Alert/TorrentPluginStartedAlert.cpp \
    src/extension/Alert/SellerTorrentPluginStatusAlert.cpp \
    src/extension/Alert/StartedSellerTorrentPlugin.cpp \
    src/extension/Alert/StartedBuyerTorrentPlugin.cpp \
    src/extension/Alert/BuyerPeerAddedAlert.cpp \
    src/extension/Alert/SellerPeerAddedAlert.cpp \
    src/extension/Alert/BuyerPeerPluginRemovedAlert.cpp \
    src/extension/Alert/SellerPeerPluginRemovedAlert.cpp \
    src/extension/BuyerTorrentPlugin.cpp \
    src/extension/SellerTorrentPlugin.cpp \
    src/extension/SellerPeerPlugin.cpp \
    src/extension/BuyerPeerPlugin.cpp \
    src/controller/BuyerPeerPluginViewModel.cpp \
    src/controller/BuyerTorrentPluginViewModel.cpp \
    src/controller/ChannelViewModel.cpp \
    src/controller/PayeeViewModel.cpp \
    src/controller/PayorViewModel.cpp \
    src/controller/PeerPluginViewModel.cpp \
    src/controller/SellerPeerPluginViewModel.cpp \
    src/controller/SellerTorrentPluginViewModel.cpp \
    src/controller/TorrentPluginViewModel.cpp \
    src/controller/TorrentViewModel.cpp \
    src/controller/Stream.cpp

HEADERS += \
    include/core/controller/Controller.hpp \
    include/core/controller/ControllerTracker.hpp \
    include/core/controller/exceptions/InvalidBitSwaprStateEntryException.hpp \
    include/core/controller/exceptions/ListenOnException.hpp \
    include/core/controller/exceptions/MissingInfoHashViewRequestException.hpp \
    include/core/Config.hpp \
    include/core/logger/LoggerManager.hpp \
    include/core/logger/exceptions/DuplicateLog.hpp \
    include/core/logger/exceptions/CannnotOpenLogFile.hpp \
    include/core/extension/TorrentPlugin.hpp \
    include/core/extension/Plugin.hpp \
    include/core/extension/PeerPlugin.hpp \
    include/core/extension/PeerPluginId.hpp \
    include/core/extension/BEPSupportStatus.hpp \
    include/core/extension/PluginMode.hpp \
    include/core/extension/Request/PluginRequest.hpp \
    include/core/extension/Request/TorrentPluginRequest.hpp \
    include/core/extension/Request/PeerPluginRequest.hpp \
    include/core/extension/Request/TorrentPluginRequestType.hpp \
    include/core/extension/Request/PluginRequestType.hpp \
    include/core/extension/Request/PeerPluginRequestType.hpp \
    include/core/extension/Request/StartBuyerTorrentPlugin.hpp \
    include/core/extension/Request/StartSellerTorrentPlugin.hpp \
    include/core/extension/Request/StartObserverTorrentPlugin.hpp \
    include/core/extension/Request/ChangeDownloadLocation.hpp \
    include/core/extension/Message/ExtendedMessageIdMapping.hpp \
    include/core/extension/Message/MessageType.hpp \
    include/core/extension/Message/ExtendedMessagePayload.hpp \
    include/core/extension/Message/JoiningContract.hpp \
    include/core/extension/Message/SignRefund.hpp \
    include/core/extension/Message/RefundSigned.hpp \
    include/core/extension/Message/Sell.hpp \
    include/core/extension/Message/Observe.hpp \
    include/core/extension/Message/JoinContract.hpp \
    include/core/extension/Message/Buy.hpp \
    include/core/extension/Message/Payment.hpp \
    include/core/extension/Message/Ready.hpp \
    include/core/extension/Message/NoPayloadMessage.hpp \
    include/core/extension/Message/RequestFullPiece.hpp \
    include/core/extension/Message/FullPiece.hpp \
    include/core/extension/Message/ExtendedMessageTools.hpp \
    include/core/extension/PeerAction.hpp \
    include/core/extension/SellModePeerAction.hpp \
    include/core/extension/BuyModePeerAction.hpp \
    include/core/extension/BuyerTorrentPlugin.hpp \
    include/core/extension/SellerTorrentPlugin.hpp \
    include/core/extension/BuyerPeerPlugin.hpp \
    include/core/extension/SellerPeerPlugin.hpp \
    include/core/extension/Alert/BuyerPeerAddedAlert.hpp \
    include/core/extension/Alert/SellerPeerAddedAlert.hpp \
    include/core/extension/Alert/BuyerPeerPluginRemovedAlert.hpp \
    include/core/extension/Alert/SellerPeerPluginRemovedAlert.hpp \
    include/core/extension/Alert/TorrentPluginAlert.hpp \
    include/core/extension/Alert/AlertTypes.hpp \
    include/core/extension/Alert/PluginStatusAlert.hpp \
    include/core/extension/Alert/BuyerTorrentPluginStatusAlert.hpp \
    include/core/extension/Alert/TorrentPluginStartedAlert.hpp \
    include/core/extension/Alert/SellerTorrentPluginStatusAlert.hpp \
    include/core/extension/Alert/StartedSellerTorrentPlugin.hpp \
    include/core/extension/Alert/StartedBuyerTorrentPlugin.hpp \
    include/core/controller/PluginInstalled.hpp \
    include/core/controller/BuyerPeerPluginViewModel.hpp \
    include/core/controller/BuyerTorrentPluginViewModel.hpp \
    include/core/controller/ChannelViewModel.hpp \
    include/core/controller/PayeeViewModel.hpp \
    include/core/controller/PayorViewModel.hpp \
    include/core/controller/PeerPluginViewModel.hpp \
    include/core/controller/SellerPeerPluginViewModel.hpp \
    include/core/controller/SellerTorrentPluginViewModel.hpp \
    include/core/controller/TorrentPluginViewModel.hpp \
    include/core/controller/TorrentViewModel.hpp \
    include/core/controller/Stream.hpp

# paymentchannel ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/lib/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/lib/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../paymentchannel/lib/ -lpaymentchannel

INCLUDEPATH += $$PWD/../paymentchannel/lib/include
DEPENDPATH += $$PWD/../paymentchannel/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/lib/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/lib/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/lib/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/lib/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/lib/libpaymentchannel.a

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

# wallet ###############################################################
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

# common ###############################################################
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

include(../libtorrent.pri)
include(../mSIGNA.pri)
include(../openssl.pri)
include(../boost.pri)
