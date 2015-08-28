# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(../libtorrent.pri) # THERE IS ALMOST NOE LIBTORRENT CODE HERE, SO PERHAPS FACTOR OUT LATER
include(../mSIGNA.pri)
include(../openssl.pri)
include(../boost.pri)

TARGET = common
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network # used by blockcypher module

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/common/DataSizeRepresentation.hpp \
    include/common/Fiat.hpp \
    include/common/Network.hpp \
    include/common/AddressType.hpp \
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
    include/common/UnspentP2PKHOutput.hpp \
    include/common/Utilities.hpp \
    include/common/TimeTools.hpp \
    include/common/Bitcoin.hpp \
    include/common/BitcoinDisplaySettings.hpp \
    include/common/BitcoinRepresentation.hpp \
    include/common/UCharArray.hpp \
    include/common/typesafeOutPoint.hpp \
    include/common/Payment.hpp \
    include/common/P2SHAddress.hpp \
    include/common/PubKeyHash.hpp \
    include/common/Seed.hpp \
    include/common/LibtorrentUtilities.hpp \
    include/common/BlockCypher.hpp

SOURCES += \
    src/DataSizeRepresentation.cpp \
    src/PublicKey.cpp \
    src/PrivateKey.cpp \
    src/Base58CheckEncodable.cpp \
    src/Signature.cpp \
    src/KeyPair.cpp \
    src/P2PKHAddress.cpp \
    src/UnspentP2PKHOutput.cpp \
    src/Utilities.cpp \
    src/TimeTools.cpp \
    src/BitcoinDisplaySettings.cpp \
    src/BitcoinRepresentation.cpp \
    src/typesafeOutPoint.cpp \
    src/Payment.cpp \
    src/P2SHAddress.cpp \
    src/Seed.cpp \
    src/LibtorrentUtilities.cpp \
    src/Network.cpp \
    src/BlockCypher.cpp
