/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/PublicKeyCompression.hpp>
#include <common/Base58CheckEncodable.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/Signature.hpp>
#include <common/TransactionSignature.hpp>
#include <common/SigHashType.hpp>
#include <common/Utilities.hpp> // sighash

#include <CoinCore/Base58Check.h>
#include <CoinCore/secp256k1_openssl.h>

#include <QDebug>

namespace Coin {

PrivateKey::PrivateKey()
    : UCharArray<PRIVATE_KEY_BYTE_LENGTH>() {
}

PrivateKey::PrivateKey(const uchar_vector & vector)
    : UCharArray<PRIVATE_KEY_BYTE_LENGTH>(vector) {
}

PrivateKey::PrivateKey(const QByteArray & array)
    : UCharArray<PRIVATE_KEY_BYTE_LENGTH>(array) {
}

PrivateKey::~PrivateKey() {
    clear();
}

PrivateKey PrivateKey::generate() {

    // Create fresh private keys
    CoinCrypto::secp256k1_key key;

    // Generate key
    key.newKey();

    // Create private key from raw uchar data
    return PrivateKey(key.getPrivKey());
}

PrivateKey PrivateKey::fromWIF(const QString & encoded) {

    // Decode string
    Base58CheckEncodable encodedType;
    Network network;
    uchar_vector payload;

    // Throws exception if Qstring is not wellformed encoding of Base58CheckEncodable type,
    // check payload length as well.
    decodeBase58CheckEncoding(encoded.toStdString(), encodedType, network, payload);

    // Check that it is indeed a WIF encoded private key
    if(encodedType != Base58CheckEncodable::WIF_PRIVATE_KEY)
        throw std::runtime_error("Encoding was not private key.");

    // Use payload length to determine if uncompression byte needs to be removed
    uchar_vector::size_type payloadLength = payload.size();

    if(payloadLength == WIF_PRIVATE_KEY_FOR_UNCOMPRESSED_PUBKEY_PAYLOAD_LENGTH) {

        Q_ASSERT(payload.size() == PRIVATE_KEY_BYTE_LENGTH);

        return PrivateKey(payload);
    } else if(payloadLength == WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_PAYLOAD_LENGTH) {

        // Check that the compressed pubkey byte has correct value
        unsigned char compressedByte = payload.back();

        if(compressedByte != WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_BYTE) {

            // Create error message and throw
            std::stringstream s;

            s << "WIF encoded privat keys corresponding to compressed pubkeys should have extra postfixed payload byte with value "
              << WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_BYTE
              << ", however value "
              << compressedByte
              << " was found instead.";

            throw std::runtime_error(s.str());
        }

        // Remove last byte WIF in payload
        payload.pop_back();

        Q_ASSERT(payload.size() == PRIVATE_KEY_BYTE_LENGTH);

        return PrivateKey(payload);
    } else
        Q_ASSERT(false);
}

QString PrivateKey::toWIF(Network network, PublicKeyCompression compression) const {

    // Create version bytes
    std::vector<unsigned char> versionBytes = toVersionBytes(Base58CheckEncodable::WIF_PRIVATE_KEY, network);

    // Create payload
    std::vector<unsigned char> payload;

    for(int i = 0;i < size();i++)
        payload.push_back(at(i));

    // Add 1 byte indicator if key corresponds to compressed pubkey
    if(compression == PublicKeyCompression::Compressed)
        payload.push_back(WIF_PRIVATE_KEY_FOR_COMPRESSED_PUBKEY_BYTE);
    else Q_ASSERT(false); // We really should only ever have private keys corresponding to compressed public keys at the moment

    // Base58Check encode and return result
    std::string encoded = toBase58Check(payload, versionBytes);

    return QString::fromStdString(encoded);
}

Signature PrivateKey::sign(const uchar_vector & data) const {

    // Create signing key
    CoinCrypto::secp256k1_key signingKey;
    signingKey.setPrivKey(toUCharVector());

    // Comute signature and return
    return Signature(CoinCrypto::secp256k1_sign(signingKey, data));
}

TransactionSignature PrivateKey::sign(const Coin::Transaction & tx, uint inputToSign, const uchar_vector & scriptPubKey, const SigHashType & type) const {

    // Generate sighash
    bytes_t hash = sighash(tx, inputToSign, scriptPubKey, type);

    //qDebug() << QString::fromStdString(uchar_vector(hash).getHex());

    // Create signature and return
    return TransactionSignature(sign(hash), type);
}

TransactionSignature PrivateKey::signForP2PKHSpend(const Coin::Transaction & tx, uint inputToSign) const {
    return sign(tx, inputToSign, P2PKHScriptPubKey(toPublicKey().toPubKeyHash()).serialize(), SigHashType::standard());
}

PublicKey PrivateKey::toPublicKey() const {

    // Wrap in key class
    CoinCrypto::secp256k1_key sk;
    sk.setPrivKey(this->toUCharVector());

    // Convert to compressed public key
    bytes_t publicKey = sk.getPubKey(true);

    return PublicKey(publicKey);
}

}
