/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SIGN_REFUND_HPP
#define SIGN_REFUND_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/TxId.hpp"
#include "extension/BitCoin/PublicKey.hpp"

#include <QtGlobal>

class SignRefund : public ExtendedMessagePayload
{
public:

    // Default constructor
    SignRefund();

    // Constructor from members
    SignRefund(const TxId & hash, quint32 index, quint64 value, const PublicKey & contractPk, const PublicKey & finalPk);

    // Constructor based on raw payload
    SignRefund(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    MessageType messageType() const;
    quint32 length() const;
    void write(QDataStream & stream) const;

    // Getters and setters
    TxId hash() const;
    void setHash(const TxId & hash);

    quint32 index() const;
    void setIndex(quint32 index);

    quint64 value() const;
    void setValue(quint64 value);

    PublicKey contractPk() const;
    void setContractPk(const PublicKey & contractPk);

    PublicKey finalPk() const;
    void setFinalPk(const PublicKey & finalPk);

private:

    // Contract hash
    TxId _hash;

    // Contract output
    quint32 _index;

    // Contract output value
    quint64 _value;

    // Contract output buyer multisig key
    PublicKey _contractPk;

    // Payment/Refund buyer output
    PublicKey _finalPk;
};

#endif // SIGN_REFUND_HPP
