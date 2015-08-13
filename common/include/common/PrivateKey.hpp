/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef PRIVATE_KEY_HPP
#define PRIVATE_KEY_HPP

#include <common/UCharArray.hpp>
#include <common/Network.hpp>
#include <common/PublicKey.hpp>
#include <common/PublicKeyCompression.hpp>

#define PRIVATE_KEY_BYTE_LENGTH 32

// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR

namespace Coin {

// Later make the allocation/copying anti-page secure
class PrivateKey : public UCharArray<PRIVATE_KEY_BYTE_LENGTH> {

public:

    // Default constructor with blank key
    PrivateKey();

    // Constructor from raw key
    PrivateKey(const uchar_vector & vector);

    // Secure destruction private key
    ~PrivateKey();

    // Factory from WIF encoded private key (for compressed pubkey)
    static PrivateKey fromWIF(const QString & encoded);

    // WIF Encode private key
    QString toWIF(Network network, PublicKeyCompression compression) const;

    PublicKey derivePublicKey() const;
};

}

#endif // PRIVATE_KEY_HPP
