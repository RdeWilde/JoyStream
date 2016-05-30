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

#define PRIVATE_KEY_BYTE_LENGTH 32

namespace Coin {

class PublicKey;
class Signature;
class TransactionSignature;
class Transaction;
enum class PublicKeyCompression;
class SigHashType;

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

    // Validity of key
    static bool valid(const PrivateKey &);

    // WIF Encode private key
    QString toWIF(Network network, PublicKeyCompression compression) const;

    // Sign raw data
    Signature sign(const uchar_vector & message) const;

    // Sign transaction in given sighash mode on given input for spending
    // output with given output script
    TransactionSignature sign(const Coin::Transaction & tx, uint inputToSign, const uchar_vector & scriptPubKey, const SigHashType & type) const;

    // Sign transaction in SIGHASH_ALL mode on given input for spending
    // output with p2kh output controlled by this private key
    TransactionSignature signForP2PKHSpend(const Coin::Transaction & tx, uint inputToSign) const;

    // Generates the correponding (compressed) public key
    PublicKey toPublicKey() const;
};

class InvalidPrivateKeyException : std::runtime_error {

public:

    InvalidPrivateKeyException(const PrivateKey & sk)
        : std::runtime_error("Invalid private key.")
        , _sk(sk) {
    }

    PrivateKey sk() const {
        return _sk;
    }

private:

    PrivateKey _sk;
};


}

#endif // COIN_PRIVATE_KEY_HPP
