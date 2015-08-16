/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <common/P2PKHAddress.hpp>
#include <common/Base58CheckEncodable.hpp>
#include <CoinCore/Base58Check.h>

namespace Coin {

P2PKHAddress::P2PKHAddress(Network network, const UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> & pubKeyHash)
    : _network(network)
    , _pubKeyHash(pubKeyHash) {
}

// Base58CheckEncoded p2pkh address
P2PKHAddress P2PKHAddress::fromBase58CheckEncoding(const QString & encoded) {

    // Decode
    Base58CheckEncodable encodedType;
    Network network;
    uchar_vector pubKeyHash;

    decodeBase58CheckEncoding(encoded.toStdString(), encodedType, network, pubKeyHash);

    // Check that input is indeed a p2pkh address
    if(encodedType != Base58CheckEncodable::P2PKH_ADDRESS)
        throw std::runtime_error("Argument was not a p2pkh address.");

    // Create address and return
    return P2PKHAddress(network, pubKeyHash);
}

QString P2PKHAddress::toBase58CheckEncoding() const {

    // Create version bytes
    std::vector<unsigned char> versionBytes = toVersionBytes(Base58CheckEncodable::P2PKH_ADDRESS, _network);

    // Base58Check encode and return result
    std::string encoded = toBase58Check(_pubKeyHash.toUCharVector(), versionBytes);

    return QString::fromStdString(encoded);
}

Network P2PKHAddress::network() const {
    return _network;
}

void P2PKHAddress::setNetwork(Network network) {
    _network = network;
}

UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> P2PKHAddress::pubKeyHash() const {
    return _pubKeyHash;
}

void P2PKHAddress::setPubKeyHash(const UCharArray<P2PKH_ADDRESS_PAYLOAD_BYTE_LENGTH> & pubKeyHash) {
    _pubKeyHash = pubKeyHash;
}

}
