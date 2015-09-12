# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

TARGET = gui
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core network sql gui # network is needed due to tcpservers/clients in, sql is needed for wallet

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
    src/UtxoEventViewModel.cpp

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
    include/gui/UtxoEventViewModel.hpp

FORMS += \
    ui/SellerTorrentPluginConfigurationDialog.ui \
    ui/BuyerTorrentPluginConfigurationDialog.ui \
    ui/PeerPluginsDialognsdialog.ui \
    ui/SellerTorrentPluginDialog.ui \
    ui/BuyerTorrentPluginDialog.ui \
    ui/MainWindow.ui \
    ui/AddTorrentDialog.ui \
    ui/WalletDialog.ui

RESOURCES += \
    base.qrc


# paymentchannel ###############################################################
INCLUDEPATH += $$PWD/../../paymentchannel/lib/include

# blockcypher  ###############################################################
INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher/include

# wallet ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../wallet/lib/release/ -lwallet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../wallet/lib/debug/ -lwallet
else:unix: LIBS += -L$$OUT_PWD/../../wallet/lib/ -lwallet

INCLUDEPATH += $$PWD/../../wallet/lib/include
DEPENDPATH += $$PWD/../../wallet/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/lib/release/libwallet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/lib/debug/libwallet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/lib/release/wallet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../wallet/lib/debug/wallet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../wallet/lib/libwallet.a

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
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/lib/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/lib/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/lib/ -lcommon

INCLUDEPATH += $$PWD/../../common/lib/include
DEPENDPATH += $$PWD/../../common/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/lib/libcommon.a

include(../../mSIGNA.pri) # needed for <stdutils/uchar_vector.h>
include(../../libtorrent.pri)
include(../../boost.pri)
