/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <common/TransactionId.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <algorithm>

namespace Coin {

TransactionId::TransactionId() {
}

TransactionId TransactionId::fromTx(const Coin::Transaction & tx) {

    uchar_vector v(tx.getHashLittleEndian());

    return TransactionId::fromRPCByteOrder(v);
}

/**
TransactionId::TransactionId(const Coin::Transaction & tx)
    : UCharArray<TXID_BYTE_LENGTH>(tx.getHashLittleEndian()) { // we need RPC bye order, which coincore calls little endian, *incorrectly*
}
*/

TransactionId TransactionId::fromInternalByteOrder(const uchar_vector & vector) {

    // Check length
    if(vector.size() != TXID_BYTE_LENGTH)
        throw std::runtime_error("Incorrect size");

    // Make copy so we can modify it
    uchar_vector copy(vector);

    // Convert to RPC byte order
    copy.reverse();

    // Create blank txid
    TransactionId id;

    // Fill id with copy
    id.fill(copy.data());

    return id;
}

TransactionId TransactionId::fromRPCByteOrder(const std::string & str) {

    // Turn std::string into QByteArray
    QByteArray hexArray = QByteArray::fromStdString(str);

    // Use QbyteArray factory
    return TransactionId::fromRPCByteOrder(QByteArray::fromHex(hexArray));
}

TransactionId TransactionId::fromRPCByteOrder(const QByteArray & array) {

    // Check length
    if(array.size() != TXID_BYTE_LENGTH)
        throw std::runtime_error("Incorrect size");

    // Create blank txid
    TransactionId id;

    // Fill id with copy
    id.fill((const unsigned char *)array.data());

    return id;
}

TransactionId TransactionId::fromRPCByteOrder(const uchar_vector & vector) {

    QByteArray array((const char *)vector.data(), (int)(vector.size()));

    return TransactionId::fromRPCByteOrder(array);
}

std::string TransactionId::toRPCByteOrder() const {

    // Copy raw into buffer
    uchar_vector tmp = toUCharVector();

    // Turn into hex string
    return tmp.getHex();
}

}
