# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

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
    include/common/UnspentOutput.hpp \
    include/common/UnspentP2SHOutput.hpp \
    include/common/UnspentP2PKHOutput.hpp \
    include/common/Utilities.hpp \
    include/common/TimeTools.hpp \
    include/common/Bitcoin.hpp \
    include/common/BitcoinDisplaySettings.hpp \
    include/common/BitcoinRepresentation.hpp \
    include/common/UCharArray.hpp \
    include/common/typesafeOutPoint.hpp \
    include/common/P2SHAddress.hpp \
    include/common/PubKeyHash.hpp \
    include/common/Seed.hpp \
    include/common/Entropy.hpp \
    include/common/LibtorrentUtilities.hpp \
    include/common/RedeemScriptHash.hpp \
    include/common/SigHashType.hpp \
    include/common/MultisigScriptPubKey.hpp \
    include/common/P2SHScriptPubKey.hpp \
    include/common/MultisigScriptSig.hpp \
    include/common/TransactionSignature.hpp \
    include/common/P2SHScriptSig.hpp \
    include/common/P2PKHScriptPubKey.hpp \
    include/common/P2PKHScriptSig.hpp \
    include/common/MajorMinorSoftwareVersion.hpp \
    include/common/P2PKScriptPubKey.hpp \
    include/common/Payment.hpp \
    include/common/UnspentOutputSet.hpp \
    include/common/Script.hpp

SOURCES += \
    src/DataSizeRepresentation.cpp \
    src/PublicKey.cpp \
    src/PrivateKey.cpp \
    src/Base58CheckEncodable.cpp \
    src/Signature.cpp \
    src/KeyPair.cpp \
    src/P2PKHAddress.cpp \
    src/UnspentOutput.cpp \
    src/UnspentP2SHOutput.cpp \
    src/UnspentP2PKHOutput.cpp \
    src/Utilities.cpp \
    src/TimeTools.cpp \
    src/BitcoinDisplaySettings.cpp \
    src/BitcoinRepresentation.cpp \
    src/typesafeOutPoint.cpp \
    src/P2SHAddress.cpp \
    src/Seed.cpp \
    src/Entropy.cpp \
    src/LibtorrentUtilities.cpp \
    src/Network.cpp \
    src/SigHashType.cpp \
    src/MultisigScriptPubKey.cpp \
    src/P2SHScriptPubKey.cpp \
    src/MultisigScriptSig.cpp \
    src/TransactionSignature.cpp \
    src/P2SHScriptSig.cpp \
    src/P2PKHScriptPubKey.cpp \
    src/P2PKHScriptSig.cpp \
    src/P2PKScriptPubKey.cpp \
    src/Payment.cpp \
    src/TransactionId.cpp \
    src/UnspentOutputSet.cpp \
    src/RedeemScriptHash.cpp

include(../config.pri)
