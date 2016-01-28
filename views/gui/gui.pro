# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

TARGET = gui
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core
QT      += network # network is needed due to tcpservers/clients in, sql is needed for wallet
QT      += sql
QT      += gui
QT      += multimedia        # multimedia
QT      += multimediawidgets # QVideoWidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

INCLUDEPATH += $$PWD/include

SOURCES += \
    src/MainWindow.cpp \
    src/WalletDialog.cpp \
    src/SellerTorrentPluginConfigurationDialog.cpp \
    src/BuyerTorrentPluginConfigurationDialog.cpp \
    src/SellerTorrentPluginDialog.cpp \
    src/BuyerTorrentPluginDialog.cpp \
    src/AddTorrentDialog.cpp \
    src/TorrentView.cpp \
    src/ChannelView.cpp \
    src/BuyerPeerPluginView.cpp \
    src/SellerPeerPluginView.cpp \
    src/PeerPluginsDialog.cpp \
    src/UtxoEventViewModel.cpp \
    src/ReceiveFundsDialog.cpp \
    src/MediaPlayerDialog.cpp \
    src/FundingWalletProgressDialog.cpp

HEADERS += \
    include/gui/MainWindow.hpp \
    include/gui/AddTorrentDialog.hpp \
    include/gui/SellerTorrentPluginConfigurationDialog.hpp \
    include/gui/BuyerTorrentPluginConfigurationDialog.hpp \
    include/gui/WalletDialog.hpp \
    include/gui/PeerPluginsDialog.hpp \
    include/gui/SellerTorrentPluginDialog.hpp \
    include/gui/BuyerTorrentPluginDialog.hpp \
    include/gui/TorrentView.hpp \
    include/gui/ChannelView.hpp \
    include/gui/BuyerPeerPluginView.hpp \
    include/gui/SellerPeerPluginView.hpp \
    include/gui/UtxoEventViewModel.hpp \
    include/gui/ReceiveFundsDialog.hpp \
    include/gui/MediaPlayerDialog.hpp \
    include/gui/FundingWalletProgressDialog.hpp

FORMS += \
    ui/SellerTorrentPluginConfigurationDialog.ui \
    ui/BuyerTorrentPluginConfigurationDialog.ui \
    ui/PeerPluginsDialognsdialog.ui \
    ui/SellerTorrentPluginDialog.ui \
    ui/BuyerTorrentPluginDialog.ui \
    ui/MainWindow.ui \
    ui/AddTorrentDialog.ui \
    ui/WalletDialog.ui \
    ui/ReceiveFundsDialog.ui \
    ui/MediaPlayerDialog.ui \
    ui/FundingWalletProgressDialog.ui

RESOURCES += \
    base.qrc

# paymentchannel
INCLUDEPATH += $$PWD/../../paymentchannel/include
DEPENDPATH += $$PWD/../../paymentchannel/include

# blockcypher
INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher/include

# wallet
INCLUDEPATH += $$PWD/../../wallet/include
DEPENDPATH += $$PWD/../../wallet/include

# core
INCLUDEPATH += $$PWD/../../core/include
DEPENDPATH += $$PWD/../../core/include

# common
INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include

include(../../config.pri)
include(../../app/config.pri)
