/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include "ExtendedMessagePayload.hpp"
#include <common/Signature.hpp>

class Payment : public ExtendedMessagePayload
{
public:

    // Defualt constructor
    Payment();

    // Constructor from members
    Payment(const Coin::Signature & sig);

    // Constructor based on raw payload
    Payment(QDataStream & stream, quint8 lengthOfSignature);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    Coin::Signature sig() const;
    void setSig(const Coin::Signature & sig);

private:

    // Buyer payment signature
    Coin::Signature _sig;
};

#endif // PAYMENT_HPP
