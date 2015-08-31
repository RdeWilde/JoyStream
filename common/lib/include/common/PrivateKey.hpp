/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_PRIVATE_KEY_HPP
#define COIN_PRIVATE_KEY_HPP

#include <common/UCharArray.hpp>
#include <common/Network.hpp>
#include <common/PublicKeyCompression.hpp>

#define PRIVATE_KEY_BYTE_LENGTH 32

namespace Coin {

class PublicKey;

// Later make the allocation/copying anti-page secure
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
class PrivateKey : public UCharArray<PRIVATE_KEY_BYTE_LENGTH> {

public:

    // Default constructor with blank key
    PrivateKey();

    // Constructor from raw key
    PrivateKey(const uchar_vector & vector);
    PrivateKey(const QByteArray & array);

    // Secure destruction private key
    ~PrivateKey();

    // Generates fresh key
    static PrivateKey generate();

    // Factory from WIF encoded private key (for compressed pubkey)
    static PrivateKey fromWIF(const QString & encoded);

    // WIF Encode private key
    QString toWIF(Network network, PublicKeyCompression compression) const;

    // Generates the correponding (compressed) public key
    PublicKey toPublicKey() const;
};

}

#endif // COIN_PRIVATE_KEY_HPP
