/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef COMMITMENT_HPP
#define COMMITMENT_HPP

#include <common/PublicKey.hpp>

namespace Coin {

class StandardTxOut;

// Commitment of participants in a channel
// I DONT THINK THIS IS NEEDED, REMOVE LATER?
class Commitment
{
public:

    // Default constructor
    Commitment();

    // Constructor based on members
    Commitment(quint64 value, const PublicKey & firstPk, const PublicKey & secondPk);

    // Copy constructor
    Commitment(const Commitment& o);

    // Assignement operator
    Commitment & operator=(const Commitment & o);

    // Corresponding P2SH 2-2 multisig address
    //Coin::P2SHAddress toP2SHAddress(Network network) const;

    // Corresponding transaction output
    Coin::StandardTxOut toTxOut() const;

    // Getters and setters
    quint64 value() const;
    void setValue(quint64 value);

    PublicKey firstPk() const;
    void setFirstPk(const PublicKey & firstPk);

    PublicKey secondPk() const;
    void setSecondPk(const PublicKey & secondPk);

private:

    // Funds allocated to output
    quint64 _value;

    // First public key controlling multisig output
    PublicKey _firstPk;

    // Second public key controlling multisig output
    PublicKey _secondPk;
};

}

#endif // COMMITMENT_HPP
