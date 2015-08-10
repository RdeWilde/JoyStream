/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOINING_CONTRACT_HPP
#define JOINING_CONTRACT_HPP

#include "ExtendedMessagePayload.hpp"
#include <common/PublicKey.hpp>

class JoiningContract : public ExtendedMessagePayload
{
public:

    // Default constructor
    JoiningContract();

    // Construct from members
    JoiningContract(const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk);

    // Constructor based on raw payload
    JoiningContract(QDataStream & stream);

    // Getter
    Coin::PublicKey contractPk() const;
    Coin::PublicKey finalPk() const;

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Key for seller output in contract
    Coin::PublicKey _contractPk;

    // Key for payment to seller
    Coin::PublicKey _finalPk;
};

#endif // JOINING_CONTRACT_HPP
