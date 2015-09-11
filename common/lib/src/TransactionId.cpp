/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <common/TransactionId.hpp>

namespace Coin {

TransactionId::TransactionId() {
}

TransactionId::TransactionId(const uchar_vector & vector)
    : UCharArray<TXID_BYTE_LENGTH>(vector) {
}

TransactionId::TransactionId(const QByteArray & array)
    : UCharArray<TXID_BYTE_LENGTH>(array) {
}

TransactionId TransactionId::fromLittleEndianHex(const std::string & str) {

    // Copy raw into uchar buffer
    uchar_vector tmp(str);

    // Reverse from little to big endian
    tmp.reverse();

    // Create transaction id and return
    return TransactionId(tmp);
}

std::string TransactionId::toLittleEndianHex() const {

    // Copy raw into buffer
    uchar_vector tmp = toUCharVector();

    // Reverse from big to little endian
    tmp.reverse();

    // Turn into hex string
    return tmp.getHex();
}

}
