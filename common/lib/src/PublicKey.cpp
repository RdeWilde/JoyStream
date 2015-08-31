/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/PublicKey.hpp>
#include <CoinCore/hash.h> // ripemd160(sha256(pubkey))

namespace Coin {

PublicKey::PublicKey() {
}

PublicKey::PublicKey(const uchar_vector & rawCompressedKey)
    : UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH>(rawCompressedKey) {
}

PubKeyHash PublicKey::toPubKeyHash() const {
    return ripemd160(sha256(this->toUCharVector()));
}

}
