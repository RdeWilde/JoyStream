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
    src/SellerTorrentPluginConfigurationDialog.cpp \
    src/BuyerTorrentPluginConfigurationDialog.cpp \
    src/AddTorrentDialog.cpp \
    #src/UtxoEventViewModel.cpp \
    src/ReceiveFundsDialog.cpp \
    src/MediaPlayerDialog.cpp \
    src/FundingWalletProgressDialog.cpp \
    src/GeneralLoadingProgressDialog.cpp \
    src/TorrentContextMenu.cpp \
    src/Language.cpp \
    src/SellingModeSessionWidget.cpp \
    src/BuyingModeSessionWidget.cpp \
    src/TorrentTableViewRow.cpp \
    src/SessionDialog.cpp \
    src/PeersDialog.cpp

HEADERS += \
    include/gui/MainWindow.hpp \
    include/gui/AddTorrentDialog.hpp \
    include/gui/SellerTorrentPluginConfigurationDialog.hpp \
    include/gui/BuyerTorrentPluginConfigurationDialog.hpp \
    #include/gui/UtxoEventViewModel.hpp \
    include/gui/ReceiveFundsDialog.hpp \
    include/gui/MediaPlayerDialog.hpp \
    include/gui/FundingWalletProgressDialog.hpp \
    include/gui/GeneralLoadingProgressDialog.hpp \
    include/gui/TorrentContextMenu.hpp \
    include/gui/Language.hpp \
    include/gui/SellingModeSessionWidget.hpp \
    include/gui/BuyingModeSessionWidget.hpp \
    include/gui/TorrentTableViewRow.hpp \
    include/gui/SessionDialog.hpp \
    include/gui/PeersDialog.hpp

FORMS += \
    ui/SellerTorrentPluginConfigurationDialog.ui \
    ui/BuyerTorrentPluginConfigurationDialog.ui \
    ui/MainWindow.ui \
    ui/AddTorrentDialog.ui \
    ui/ReceiveFundsDialog.ui \
    ui/MediaPlayerDialog.ui \
    ui/FundingWalletProgressDialog.ui \
    ui/GeneralLoadingProgressDialog.ui \
    ui/BuyingModeSessionWidget.ui \
    ui/SellingModeSessionWidget.ui \
    ui/SessionDialog.ui \
    ui/PeersDialog.ui

RESOURCES += \
    base.qrc

INCLUDEPATH += $$PWD/../../paymentchannel/include
DEPENDPATH += $$PWD/../../paymentchannel/include

INCLUDEPATH += $$PWD/../../core/include
DEPENDPATH += $$PWD/../../core

INCLUDEPATH += $$PWD/../../extension/include
DEPENDPATH += $$PWD/../../extension

INCLUDEPATH += $$PWD/../../protocol_session/include
DEPENDPATH += $$PWD/../../protocol_session

INCLUDEPATH += $$PWD/../../protocol_statemachine/include
DEPENDPATH += $$PWD/../../protocol_statemachine

INCLUDEPATH += $$PWD/../../protocol_wire/include
DEPENDPATH += $$PWD/../../protocol_wire

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common

INCLUDEPATH += $$PWD/../../bitcoin/include
DEPENDPATH += $$PWD/../../bitcoin

include(../../config.pri)
