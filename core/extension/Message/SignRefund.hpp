/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SIGN_REFUND_HPP
#define SIGN_REFUND_HPP

#include "ExtendedMessagePayload.hpp"

#include <common/TransactionId.hpp>
#include <common/PublicKey.hpp>

#include <QtGlobal>

class SignRefund : public ExtendedMessagePayload
{
public:

    // Default constructor
    SignRefund();

    // Constructor from members
    SignRefund(const Coin::TransactionId & txid, quint32 index, quint64 value, const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk);

    // Constructor based on raw payload
    SignRefund(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    MessageType messageType() const;
    quint32 length() const;
    void write(QDataStream & stream) const;

    // Getters and setters
    quint32 index() const;
    void setIndex(quint32 index);

    quint64 value() const;
    void setValue(quint64 value);

    Coin::PublicKey contractPk() const;
    void setContractPk(const Coin::PublicKey & contractPk);

    Coin::PublicKey finalPk() const;
    void setFinalPk(const Coin::PublicKey & finalPk);

    Coin::TransactionId contractTxId() const;
    void setContractTxId(const Coin::TransactionId &contractTxId);

private:

    // Contract txid
    Coin::TransactionId _contractTxId;

    // Contract output
    quint32 _index;

    // Contract output value
    quint64 _value;

    // Contract output buyer multisig key
    Coin::PublicKey _contractPk;

    // Payment/Refund buyer output
    Coin::PublicKey _finalPk;
};

#endif // SIGN_REFUND_HPP
