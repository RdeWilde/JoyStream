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
    include/extension/BEPSupportStatus.hpp \
    include/extension/ExtendedMessageIdMapping.hpp \
    include/extension/request/PluginRequest.hpp \
    include/extension/request/TorrentPluginRequest.hpp \
    include/extension/request/TorrentPluginRequestType.hpp \
    include/extension/request/PluginRequestType.hpp \
    include/extension/request/StartBuyerTorrentPlugin.hpp \
    include/extension/request/StartSellerTorrentPlugin.hpp \
    include/extension/request/StartObserverTorrentPlugin.hpp \
    include/extension/request/ChangeDownloadLocation.hpp \
    include/extension/alert/BuyerPeerAddedAlert.hpp \
    include/extension/alert/SellerPeerAddedAlert.hpp \
    include/extension/alert/BuyerPeerPluginRemovedAlert.hpp \
    include/extension/alert/SellerPeerPluginRemovedAlert.hpp \
    include/extension/alert/TorrentPluginAlert.hpp \
    include/extension/alert/AlertTypes.hpp \
    include/extension/alert/PluginStatusAlert.hpp \
    include/extension/alert/BuyerTorrentPluginStatusAlert.hpp \
    include/extension/alert/TorrentPluginStartedAlert.hpp \
    include/extension/alert/SellerTorrentPluginStatusAlert.hpp \
    include/extension/alert/StartedSellerTorrentPlugin.hpp \
    include/extension/alert/StartedBuyerTorrentPlugin.hpp \
    include/extension/extension.hpp \
    include/extension/alert/alert.hpp \
    include/extension/request/request.hpp \
    include/extension/Status.hpp

SOURCES += \
    src/TorrentPlugin.cpp \
    src/Plugin.cpp \
    src/PeerPlugin.cpp \
    src/ExtendedMessageIdMapping.cpp \
    src/request/PluginRequest.cpp \
    src/request/TorrentPluginRequest.cpp \
    src/request/PeerPluginRequest.cpp \
    src/request/StartBuyerTorrentPlugin.cpp \
    src/request/StartSellerTorrentPlugin.cpp \
    src/request/StartObserverTorrentPlugin.cpp \
    src/request/ChangeDownloadLocation.cpp \
    src/alert/TorrentPluginAlert.cpp \
    src/alert/PluginStatusAlert.cpp \
    src/alert/BuyerTorrentPluginStatusAlert.cpp \
    src/alert/TorrentPluginStartedAlert.cpp \
    src/alert/SellerTorrentPluginStatusAlert.cpp \
    src/alert/StartedSellerTorrentPlugin.cpp \
    src/alert/StartedBuyerTorrentPlugin.cpp \
    src/alert/BuyerPeerAddedAlert.cpp \
    src/alert/SellerPeerAddedAlert.cpp \
    src/alert/BuyerPeerPluginRemovedAlert.cpp \
    src/alert/SellerPeerPluginRemovedAlert.cpp \

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# protocol_statemachine
INCLUDEPATH += $$PWD/../protocol_statemachine/include
DEPENDPATH += $$PWD/../protocol_statemachine/include

# protocol_wire
INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

# protocol_session
INCLUDEPATH += $$PWD/../protocol_session/include
DEPENDPATH += $$PWD/../protocol_session/include

include(../config.pri)
