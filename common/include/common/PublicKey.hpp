/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include <common/FixedUCharArray.hpp>

#define COMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33
#define UNCOMPRESSED_PUBLIC_KEY_BYTE_LENGTH 33

namespace Coin {

// Compressed public key
typedef FixedUCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> PublicKey;

/**
class PublicKey : public FixedUCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH> {

public:

    // Factory from encoded string
    static std::pair<PublicKey, Network> fromBase58CheckEncoding(const QString & encoded);

    // Encode public key
    QString toBase58CheckEncoding(Network network);
};
*/

}

//uint qHash(const Coin::PublicKey & o);

#endif // PUBLIC_KEY_HPP
