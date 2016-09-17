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
    src/MainWindow/MainWindow.cpp \
    src/MainWindow/TorrentTreeViewRow.cpp \
    src/MainWindow/TorrentContextMenu.cpp \
    #src/UtxoEventViewModel.cpp \
    src/ReceiveFundsDialog.cpp \
    src/FundingWalletProgressDialog.cpp \
    src/GeneralLoadingProgressDialog.cpp \
    src/Language.cpp \
    src/SellingModeSessionWidget.cpp \
    src/BuyingModeSessionWidget.cpp \
    src/SessionDialog.cpp \
    src/PeersDialog/PeersDialog.cpp \
    src/BuyerTermsDialog.cpp \
    src/CurrencyFieldPrefixSettings.cpp \
    src/SellerTermsDialog.cpp \
    src/SelectTorrentForDownloadDialog.cpp \
    src/TrayMenuManager.cpp

HEADERS += \
    include/gui/MainWindow/MainWindow.hpp \
    include/gui/MainWindow/TorrentTreeViewRow.hpp \
    include/gui/MainWindow/TorrentContextMenu.hpp \
    #include/gui/UtxoEventViewModel.hpp \
    include/gui/ReceiveFundsDialog.hpp \
    include/gui/FundingWalletProgressDialog.hpp \
    include/gui/GeneralLoadingProgressDialog.hpp \
    include/gui/Language.hpp \
    include/gui/SellingModeSessionWidget.hpp \
    include/gui/BuyingModeSessionWidget.hpp \
    include/gui/SessionDialog.hpp \
    include/gui/PeersDialog/PeersDialog.hpp \
    include/gui/BuyerTermsDialog.hpp \
    include/gui/CurrencyFieldPrefixSettings.hpp \
    include/gui/SellerTermsDialog.hpp \
    include/gui/SelectTorrentForDownloadDialog.hpp \
    include/gui/TrayMenuManager.hpp \
    include/gui/gui.hpp

FORMS += \
    ui/MainWindow.ui \
    ui/ReceiveFundsDialog.ui \
    ui/FundingWalletProgressDialog.ui \
    ui/GeneralLoadingProgressDialog.ui \
    ui/BuyingModeSessionWidget.ui \
    ui/SellingModeSessionWidget.ui \
    ui/SessionDialog.ui \
    ui/PeersDialog.ui \
    ui/BuyerTermsDialog.ui \
    ui/SellerTermsDialog.ui \
    ui/SelectTorrentForDownloadDialog.ui

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
