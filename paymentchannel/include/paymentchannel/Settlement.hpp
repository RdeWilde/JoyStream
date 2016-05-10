/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef PAYMENT_CHANNEL_SETTLEMENT_HPP
#define PAYMENT_CHANNEL_SETTLEMENT_HPP

#include <paymentchannel/Termination.hpp>
#include <common/Payment.hpp>
#include <common/PubKeyHash.hpp> // cannot forward declare typedef

namespace Coin {
    class Transaction;
    class typesafeOutPoint;
    class TransactionSignature;
    class PrivateKey;
}

namespace joystream {
namespace paymentchannel {

    class Settlement : public Termination {

    public:

        Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::Payment & toPayor,
                   const Coin::Payment & toPayee);

        Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::Payment & toPayor);

        // Creates to appropriate settlement given the dust
        // limit and transaction fee
        static Settlement dustLimitAndFeeAwareSettlement(const Coin::typesafeOutPoint & contractOutPoint,
                                                         const Commitment & commitment,
                                                         const Coin::PubKeyHash & payorKeyHash,
                                                         const Coin::PubKeyHash & payeeKeyHash,
                                                         quint64 funds,
                                                         quint64 paid,
                                                         quint64 fee);

        // Unsigned settelment transaction
        virtual Coin::Transaction unSignedTransaction() const;

        // Implicit fee by comparing commitment and (payor + payee) payment
        int64_t fee() const;

    private:

        // Whether to pay payor
        // When paid quantity to payee is lower than
        // sum of tx fee and dust limit, then everything
        // is spent to fees, and there is no need to pay payee in that case.
        // *** not sure if this is the best way to model this, but ok for now ***
        bool _includePayee;

        // Payment to payee
        Coin::Payment _toPayee;
    };

}
}

#endif // PAYMENT_CHANNEL_SETTLEMENT_HPP

