# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015

TARGET = blockcypher
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network # used by blockcypher module

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/blockcypher/Wallet.hpp \
    include/blockcypher/TXRef.hpp \
    include/blockcypher/Address.hpp \
    include/blockcypher/Client.hpp \
    include/blockcypher/CreateWallet.hpp \
    include/blockcypher/BlockCypher.hpp \
    include/blockcypher/GetWallet.hpp \
    include/blockcypher/AddAddressToWallet.hpp \
    include/blockcypher/TX.hpp \
    include/blockcypher/TXInput.hpp \
    include/blockcypher/ScriptType.hpp \
    include/blockcypher/TXOutput.hpp \
    include/blockcypher/AddressEndPoint.hpp \
    include/blockcypher/PushRawTransaction.hpp \
    include/blockcypher/DeleteWallet.hpp \
    include/blockcypher/BlockExplorer.hpp \
    include/blockcypher/FundWalletFromFaucet.hpp \
    include/blockcypher/Event.hpp \
    include/blockcypher/WebSocketClient.hpp \
    include/blockcypher/UTXOManager.hpp \
    include/blockcypher/UTXO.hpp \
    include/blockcypher/TxResult.hpp

SOURCES += \
    src/Wallet.cpp \
    src/Client.cpp \
    src/CreateWallet.cpp \
    src/BlockCypher.cpp \
    src/GetWallet.cpp \
    src/TXRef.cpp \
    src/TX.cpp \
    src/TXInput.cpp \
    src/ScriptType.cpp \
    src/Address.cpp \
    src/AddressEndPoint.cpp \
    src/PushRawTransaction.cpp \
    src/DeleteWallet.cpp \
    src/BlockExplorer.cpp \
    src/FundWalletFromFaucet.cpp \
    src/Event.cpp \
    src/TXOutput.cpp \
    src/WebSocketClient.cpp \
    src/UTXOManager.cpp \
    src/UTXO.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
