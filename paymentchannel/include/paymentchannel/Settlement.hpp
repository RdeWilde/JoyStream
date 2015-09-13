/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef PAYMENTCHANNEL_SETTLEMENT_HPP
#define PAYMENTCHANNEL_SETTLEMENT_HPP

#include <paymentchannel/Termination.hpp>
#include <common/Payment.hpp>

namespace Coin {
    class Transaction;
    class typesafeOutPoint;
    class TransactionSignature;
    class PrivateKey;
}

class Settlement : public Termination {

public:

    Settlement(const Coin::typesafeOutPoint & contractOutPoint,
               const Commitment & commitment,
               const Coin::Payment & toPayor,
               const Coin::Payment & toPayee);

    // Unsigned settelment transaction
    virtual Coin::Transaction unSignedTransaction() const;

    // Implicit fee by comparing commitment and (payor + payee) payment
    int64_t fee() const;

private:

    // Payment to payee
    Coin::Payment _toPayee;
};


#endif // PAYMENTCHANNEL_SETTLEMENT_HPP

