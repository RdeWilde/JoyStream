/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/Signature.hpp>

#include <CoinCore/hash.h> // ripemd160(sha256(pubkey))
#include <CoinCore/secp256k1.h> // CoinCrypto::secp256k1_key

namespace Coin {

PublicKey::PublicKey() {
}

PublicKey::PublicKey(const uchar_vector & rawCompressedKey)
    : UCharArray<COMPRESSED_PUBLIC_KEY_BYTE_LENGTH>(rawCompressedKey) {
}

PubKeyHash PublicKey::toPubKeyHash() const {
    return PubKeyHash(ripemd160(sha256(this->toUCharVector())));
}

bool PublicKey::verify(const uchar_vector & message, const Signature & sig) const {

    // Create signature checking key for thisp public key
    CoinCrypto::secp256k1_key signatureCheckingKey;
    signatureCheckingKey.setPubKey(toUCharVector());

    // Check signature
    bool verified;

    try {

        verified = CoinCrypto::secp256k1_verify(signatureCheckingKey, message, sig.toUCharVector());

    } catch(const std::runtime_error & e) {

        // We may have error if the input was rejected by openssl
        return false;
    }

    return verified;
}

P2PKHAddress PublicKey::toP2PKHAddress(Network network) const{
    return P2PKHAddress(network, toPubKeyHash());
}

}
