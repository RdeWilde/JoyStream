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

        // Get an UnspentP2SHOutput to spend the entire commitment
        // Important : The transaction which spends this output must set nSequence to equal the locktime of the commitment
        // or the OP_CHECSEQUENCEVERIFY will fail
        Coin::UnspentP2SHOutput getUnspentOutput() const;

        // Wether refund is still locked
        bool isLocked(uint32_t currentBlockHeight, uint32_t contractMinedInBlock) const;

        // At which block height refund is unlocked
        uint32_t lockedUntil(uint32_t contractMinedInBlock) const;

    private:

        // Contract to which commitment corresponds
        Coin::typesafeOutPoint _contractOutPoint;

        // Commitment being refunded
        Commitment _commitment;

        // KeyPair of payor that controls the payment channel output
        Coin::KeyPair _payorContractKeyPair;
    };

}
}

#endif // PAYMENTCHANNEL_REFUND_HPP

