/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef REFUND_SIGNED_HPP
#define REFUND_SIGNED_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/Signature.hpp"

class RefundSigned : public ExtendedMessagePayload
{
public:

    // Default constructor
    RefundSigned();

    // Construct from members
    RefundSigned(const Signature & sig);

    // Constructor based on raw payload
    RefundSigned(QDataStream & stream, quint8 lengthOfSignature);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    Signature sig() const;
    void setSig(const Signature & sig);

private:

    // Seller refund signature
    Signature _sig;
};

#endif // REFUND_SIGNED_HPP
