/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 11 2015
 */

#include <common/Seed.hpp>
#include <common/Entropy.hpp>
#include <CoinCore/hdkeys.h>
#include <CoinCore/bip39.h>

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace Coin {

Entropy::Entropy() {
}

Entropy::Entropy(const char * raw)
    : Entropy(QByteArray(raw, WALLET_ENTROPY_BYTE_LENGTH)) {
}

Entropy::Entropy(const QByteArray & raw)
    : Coin::UCharArray<WALLET_ENTROPY_BYTE_LENGTH>(raw) {
}

Entropy::Entropy(const uchar_vector &raw)
    : Coin::UCharArray<WALLET_ENTROPY_BYTE_LENGTH>(raw) {

}

Entropy::Entropy(const std::string & hexEncoded)
    : Coin::UCharArray<WALLET_ENTROPY_BYTE_LENGTH>(hexEncoded) {
}

Entropy::~Entropy(){
    clear();
}

Entropy Entropy::generate() {

    Entropy e;

    if (!RAND_bytes(&e.at(0), WALLET_ENTROPY_BYTE_LENGTH)) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
    }

    return e;
}

Seed Entropy::seed(std::string passphrase) const {
    // Convert entropy to mnemonic phrase
    std::string wordlistString = Coin::BIP39::toWordlist(this->toUCharVector());
    const char * password = wordlistString.c_str();

    std::string saltString = "mnemonic" + passphrase;
    const char * salt = saltString.c_str();

    unsigned char digest[WALLET_SEED_BYTE_LENGTH];

    /* https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki
     * To create a binary seed from the mnemonic, we use the PBKDF2 function with a mnemonic sentence
     * (in UTF-8 NFKD) used as the password and the string "mnemonic" + passphrase (again in UTF-8 NFKD)
     * used as the salt. The iteration count is set to 2048 and HMAC-SHA512 is used as the pseudo-random function.
     * The length of the derived key is 512 bits (= 64 bytes).
     */
    if(!PKCS5_PBKDF2_HMAC(password, strlen(password), (unsigned char*)salt, strlen(salt), 2048, EVP_sha512(), sizeof(digest), digest)){
        throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
    }

    // Construct Seed from raw bytes
    QByteArray seed((char*)digest, WALLET_SEED_BYTE_LENGTH);

    return Seed(seed);
}

Entropy Entropy::fromMnemonic(std::string wordList) {
    uchar_vector raw(Coin::BIP39::fromWordlist(wordList));
    return Entropy(raw.getHex());
}

std::string Entropy::mnemonic() const {
    return Coin::BIP39::toWordlist(this->toUCharVector());
}

std::string Entropy::getHex() const {
    return this->toUCharVector().getHex();
}

}
