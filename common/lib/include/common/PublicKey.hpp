/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_PUBLIC_KEY_HPP
#define COIN_PUBLIC_KEY_HPP

#include <common/UCharArray.hpp>
#include <common/PubKeyHash.hpp>

#define COMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33
#define UNCOMPRESSED_PUBLIC_KEY_BYTE_LENGTH 65

namespace Coin {

enum class Network;
class Signature;
class P2PKHAddress;

// Compresed public key
class PublicKey : public UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> {

public:

    // Defualt constructor
    PublicKey();

    // Constructor from raw compressed key
    PublicKey(const uchar_vector & rawCompressedKey);

    // Whether signature <sig> is valid for the private key corresponding
    // public key <pk> on message <message>
    bool verify(const uchar_vector & message, const Signature & sig) const;

    // Public key hash, e.g. for addresses
    PubKeyHash toPubKeyHash() const;

    // Generate corresponding ntwork
    P2PKHAddress toP2PKHAddress(Network network) const;
};

}

//uint qHash(const Coin::PublicKey & o);

#endif // COIN_PUBLIC_KEY_HPP
