/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

/**
#include <common/PublicKey.hpp>
#include <CoinCore/Base58Check.h>

using namespace Coin;

QPair<PublicKey, Network> PublicKey::fromBase58CheckEncoding(const QString & encoded) {

    std::string base58check = encoded.toStdString();

    // Buffers for parsing encoded string into
    uchar_vector payload;
    unsigned int version;

    // Try to decode string
    bool isValid = fromBase58Check(base58check, payload, version);

    // Throw error if encoding is not valid
    if(!isValid)
        throw std::runtime_error("Malformed base58CheckEncoding.");

    // Check that payload is of correct length
    uchar_vector::size_type payloadLength = payload.size();

    if(payloadLength != COMPRESSED_PUBLIC_KEY_BYTE_LENGTH) {

        // Build error message
        std::stringstream s;

        s << "Encoded payload of length "
          << payloadLength
          << ", but expected length of "
          << COMPRESSED_PUBLIC_KEY_BYTE_LENGTH;

        throw std::runtime_error(s.str());
    }

    // Decode version bytes into type and network
    std::pair<Base58CheckEncodable, Network> decoded = encodableVersionByteToBase58CheckEncodable(version);

    // Check that the version byte actually corresponds to a public key
    if(decoded.first != Base58CheckEncodable::PUBLIC_KEY)
        throw std::runtime_error("Did not encode public key.");

    // Create public key
    PublicKey pk(payload);

    // Return public key
    return std::make_pair(pk, decoded.second);
}

QString PublicKey::toBase58CheckEncoding(Network network) {

}


const int PublicKey::compressedLength = 33;
const int PublicKey::unCompressedLength = 65;

PublicKey::PublicKey(const uchar_vector & raw)
    : _raw(raw) {

    // Validate length
    std::vector<unsigned char>::size_type length = _raw.size();

    if(length != PublicKey::compressedLength &&
            length != PublicKey::unCompressedLength) {

        std::stringstream s;

        s << "Invaid public key length found, must be "
          << PublicKey::compressedLength << " (compressed) or"
          << PublicKey::unCompressedLength << " (uncompressed), but found: " << length;

        throw std::runtime_error(s.str());
    }
}

PublicKey::PublicKey(const PublicKey & publicKey)
    : _raw(publicKey.raw()) {
}

bool PublicKey::operator==(const PublicKey & rhs) {
}

bool PublicKey::operator!=(const PublicKey & rhs) {

}

bool PublicKey::isCompressed() const {
    return _raw.size() == PublicKey::compressedLength;
}

uchar_vector PublicKey::raw() const {
    return _raw;
}

RawCompressedPublicKey PublicKey::raw() const {
    return _raw;
}

void PublicKey::setRaw(const RawCompressedPublicKey & raw) {
    _raw = raw;
}
*/
