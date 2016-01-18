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
    include/blockcypher/Event.hpp

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
    src/TXOutput.cpp

# common #############################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a

include(../config.pri)
