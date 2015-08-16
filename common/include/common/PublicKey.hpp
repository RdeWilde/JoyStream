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
#define UNCOMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33

namespace Coin {

enum class Network;

// Compressed public key
//typedef UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> PublicKey;

// Compresed public key
class PublicKey : public UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> {

public:

    // Defualt constructor
    PublicKey();

    // Constructor from raw compressed key
    PublicKey(const uchar_vector & rawCompressedKey);

    // Public key hash, e.g. for addresses
    PubKeyHash toPubKeyHash() const;
};

}

//uint qHash(const Coin::PublicKey & o);

#endif // COIN_PUBLIC_KEY_HPP
