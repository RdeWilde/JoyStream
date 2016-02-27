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
    ui/ReceiveFundsDialog.ui \
    ui/MediaPlayerDialog.ui \
    ui/FundingWalletProgressDialog.ui

RESOURCES += \
    base.qrc

# paymentchannel ###############################################################
INCLUDEPATH += $$PWD/../../paymentchannel/include

# blockcypher  ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../blockcypher/release/ -lblockcypher
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../blockcypher/debug/ -lblockcypher
else:unix: LIBS += -L$$OUT_PWD/../../blockcypher/ -lblockcypher

INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/release/libblockcypher.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/debug/libblockcypher.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/release/blockcypher.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/debug/blockcypher.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/libblockcypher.a

# wallet ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../wallet/release/ -lwallet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../wallet/debug/ -lwallet
else:unix: LIBS += -L$$OUT_PWD/../../wallet/ -lwallet

INCLUDEPATH += $$PWD/../../wallet/include
DEPENDPATH += $$PWD/../../wallet/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/release/libwallet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/debug/libwallet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/release/wallet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/debug/wallet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../wallet/libwallet.a

# core ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lcore

INCLUDEPATH += $$PWD/../../core/include
DEPENDPATH += $$PWD/../../core/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../core/libcore.a

# common ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a

INCLUDEPATH += $$PWD/../../bitcoin/include
DEPENDPATH += $$PWD/../../bitcoin/include

include(../../config.pri)
include(../../app/config.pri)
