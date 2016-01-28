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
    include/core/viewmodel/BuyerPeerPluginViewModel.hpp \
    include/core/viewmodel/BuyerTorrentPluginViewModel.hpp \
    include/core/viewmodel/ChannelViewModel.hpp \
    include/core/viewmodel/PayeeViewModel.hpp \
    include/core/viewmodel/PayorViewModel.hpp \
    include/core/viewmodel/PeerPluginViewModel.hpp \
    include/core/viewmodel/SellerPeerPluginViewModel.hpp \
    include/core/viewmodel/SellerTorrentPluginViewModel.hpp \
    include/core/viewmodel/TorrentPluginViewModel.hpp \
    include/core/viewmodel/TorrentViewModel.hpp \
    include/core/Controller.hpp \
    include/core/PluginInstalled.hpp \
    include/core/Stream.hpp \
    include/core/configuration/Torrent.hpp \
    include/core/configuration/Controller.hpp

SOURCES += \
    src/logger/LoggerManager.cpp \
    src/logger/exceptions/CannnotOpenLogFile.cpp \
    src/logger/exceptions/DuplicateLog.cpp \
    src/viewmodel/BuyerPeerPluginViewModel.cpp \
    src/viewmodel/BuyerTorrentPluginViewModel.cpp \
    src/viewmodel/ChannelViewModel.cpp \
    src/viewmodel/PayeeViewModel.cpp \
    src/viewmodel/PayorViewModel.cpp \
    src/viewmodel/PeerPluginViewModel.cpp \
    src/viewmodel/SellerPeerPluginViewModel.cpp \
    src/viewmodel/SellerTorrentPluginViewModel.cpp \
    src/viewmodel/TorrentPluginViewModel.cpp \
    src/viewmodel/TorrentViewModel.cpp \
    src/Controller.cpp \
    src/Stream.cpp \
    src/configuration/Torrent.cpp \
    src/configuration/Controller.cpp

# extension
INCLUDEPATH += $$PWD/../extension/include
DEPENDPATH += $$PWD/../extension/include

# blockcypher
INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

# wallet
INCLUDEPATH += $$PWD/../wallet/include
DEPENDPATH += $$PWD/../wallet/include

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
