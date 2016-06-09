/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef PAYMENTCHANNEL_REFUND_HPP
#define PAYMENTCHANNEL_REFUND_HPP

#include <paymentchannel/Commitment.hpp>
#include <common/UnspentP2SHOutput.hpp>

namespace Coin {
    class Transaction;
    class TransactionSignature;
    class PrivateKey;
    class Payment;
}

namespace joystream {
namespace paymentchannel {

    class Commitment;

    class Refund {

    public:

        Refund(const Coin::typesafeOutPoint & contractOutPoint,
               const Commitment & commitment, const Coin::KeyPair &payorContractKeyPair);

        // Get an UnspentP2SHOutput to spend the refund
        // Important : The transaction which spends this output must set nLockTime to equal lockTime
        // or the OP_CHECKLOCKTIMEVERIFY will fail
        Coin::UnspentP2SHOutput getUnspentOutput() const;

        // Wether refund is still locked
        // If the locktime is a block height it will be compared to the currentBlockHeight argument
        // otherwise it will be compared to current time
        bool isLocked(uint32_t currentBlockHeight = 0) const;

        // when refund is unlocked
        uint32_t lockedUntil() const;

    private:

        // Contract to which commitment corresponds
        Coin::typesafeOutPoint _contractOutPoint;

        // Commitment being terminated
        Commitment _commitment;

        // KeyPair of payor that controls the payment channel output
        Coin::KeyPair _payorContractKeyPair;
    };

}
}

#endif // PAYMENTCHANNEL_REFUND_HPP

