/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 11 2015
 */

#ifndef ENTROPY_HPP
#define ENTROPY_HPP

#include <common/UCharArray.hpp>

#define WALLET_ENTROPY_BYTE_LENGTH 16 //16 bytes == 128 bits

namespace Coin {
    class Seed;

class Entropy : public Coin::UCharArray<WALLET_ENTROPY_BYTE_LENGTH> {

public:

    // Default constructor
    Entropy();

    // Constructor from raw data
    explicit Entropy(const QByteArray & raw);

    // Constructor from raw data
    explicit Entropy(const char * raw);

    // Constructor from hex encoded string
    explicit Entropy(const std::string & hexEncoded);

    static Entropy generate();

    static Entropy fromMnemonic(std::string wordList);

    // BIP39 Seed
    Seed seed(std::string passphrase = "") const;

    // Entropy encoded as BIP39 mnemonic word list
    std::string mnemonic() const;

    // Entropy as hex encoded string
    std::string getHex() const;

    // Secure destructor
    ~Entropy();
};

}

#endif // SEED_HPP
