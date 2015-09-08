/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef PAYMENTCHANNEL_REFUND_HPP
#define PAYMENTCHANNEL_REFUND_HPP

#include <paymentchannel/Termination.hpp>

class Commitment;

namespace Coin {
    class Transaction;
    class TransactionSignature;
    class PrivateKey;
    class Payment;
}

class Refund : public Termination {

public:

    Refund(const Coin::typesafeOutPoint & contractOutPoint,
           const Commitment & commitment,
           const Coin::Payment & toPayor,
           uint32_t lockTime);

    // Unsigned refund transaction
    virtual Coin::Transaction unSignedTransaction() const;

    // Implicit fee by comparing commitment and refund payment
    int64_t fee() const;

private:

    // Lock time on refund, encoded as
    uint32_t _lockTime;
};

#endif // PAYMENTCHANNEL_REFUND_HPP

