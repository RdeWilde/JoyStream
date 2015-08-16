/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#ifndef COIN_P2PKH_ADDRESS_HPP
#define COIN_P2PKH_ADDRESS_HPP

#include <common/UCharArray.hpp>
#include <common/Network.hpp>

#define P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH 20

namespace Coin {

class P2PKHAddress {

public:

    // Constructor from members
    P2PKHAddress(Network network, const UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> & pubKeyHash);

    // Factory from Base58CheckEncoding
    static P2PKHAddress fromBase58CheckEncoding(const QString & encoded);

    // Base58CheckEncode
    QString toBase58CheckEncoding() const;

    // Generate output script
    //uchar_vector toScriptPubKey() const;

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> pubKeyHash() const;
    void setPubKeyHash(const UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> & pubKeyHash);

private:

    // Network to which this address corresponds
    Network _network;

    // Pub key hash
    UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> _pubKeyHash;

};

}

#endif // COIN_P2PKH_ADDRESS_HPP
