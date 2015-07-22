# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 28 2015

include(../boost.pri)

TARGET = wallet
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # QMainWindow, QDialog

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/wallet/Wallet.hpp \
    include/wallet/Network.hpp \
    include/wallet/WalletKey.hpp \
    include/wallet/WalletKeyControllingReceiveAddress.hpp \
    include/wallet/Payee.hpp \
    include/wallet/Slot.hpp \
    include/wallet/SPVValidator.hpp \
    include/wallet/SPVWAllet.hpp \
    include/wallet/Payer.hpp \
    include/wallet/WalletEntity.hpp \
    include/wallet/CoinWrappers.hpp \
    include/wallet/AddressType.hpp \
    include/wallet/WalletAddress.hpp

SOURCES += \
    src/Wallet.cpp \
    src/WalletKey.cpp \
    src/WalletKeyControllingReceiveAddress.cpp \
    src/Payee.cpp \
    src/Slot.cpp \
    src/SPVValidator.cpp \
    src/SPVWAllet.cpp \
    src/Payer.cpp \
    src/WalletEntity.cpp \
    src/WalletAddress.cpp