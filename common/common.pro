# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(../libtorrent.pri)
include(../mSIGNA.pri)

TARGET = common
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

INCLUDEPATH += $$PWD # include in src folder

HEADERS += \
    include/common/DataSizeRepresentation.hpp \
    include/common/Fiat.hpp \
    include/common/CoinWrappers.hpp \
    include/common/Network.hpp \
    include/common/AddressType.hpp \
    include/common/FixedUCharArray.hpp \
    include/common/PrivateKey.hpp \
    include/common/PublicKey.hpp \
    include/common/Signature.hpp \
    include/common/TransactionId.hpp \
    include/common/BlockId.hpp \
    include/common/TransactionMerkleTreeRoot.hpp \
    include/common/Base58CheckEncodable.hpp \
    include/common/PublicKeyCompression.hpp \
    include/common/KeyPair.hpp \
    include/common/P2PKHAddress.hpp \
    include/common/P2PKHTxOut.hpp \
    include/common/P2SHTxOut.hpp \
    include/common/UnspentP2PKHOutput.hpp \
    include/common/Utilities.hpp \
    include/common/TimeTools.hpp \
    include/common/Bitcoin.hpp \
    include/common/BitcoinDisplaySettings.hpp \
    include/common/BitcoinRepresentation.hpp
    #include/common/RawCompressedPublicKey.hpp \
    #include/common/P2PKHAddress.hpp

SOURCES += \
    src/DataSizeRepresentation.cpp \
    src/CoinWrappers.cpp \
    # src/FixedUCharArray.cpp \
    src/PublicKey.cpp \
    src/PrivateKey.cpp \
    src/Base58CheckEncodable.cpp \
    src/Signature.cpp \
    #src/P2PKHAddress.cpp
    src/KeyPair.cpp \
    src/OutPoint.cpp \
    src/P2PKHAddress.cpp \
    src/P2PKHTxOut.cpp \
    src/P2SHTxOut.cpp \
    src/UnspentP2PKHOutput.cpp \
    src/Utilities.cpp \
    src/TimeTools.cpp \
    src/BitcoinDisplaySettings.cpp \
    src/BitcoinRepresentation.cpp

