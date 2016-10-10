/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Refund.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER, sighash
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <ctime>

namespace joystream {
namespace paymentchannel {

    Refund::Refund(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::KeyPair & payorContractKeyPair)
        : _contractOutPoint(contractOutPoint)
        , _commitment(commitment)
        , _payorContractKeyPair(payorContractKeyPair) {
    }

    Coin::UnspentP2SHOutput Refund::getUnspentOutput() const {
        // Todo: replace this with a typesafe output which is will ensure that a transaction
        // which spends it will have nSequence set to the commitment locktime

        return Coin::UnspentP2SHOutput(_payorContractKeyPair,
                                       _commitment.redeemScript().serialized(),
                                       RedeemScript::PayorOptionalData(),
                                       _contractOutPoint,
                                       _commitment.value());
    }

    uint32_t Refund::lockedUntil(uint32_t contractMinedInBlock) const {
        return _commitment.lockTime() + contractMinedInBlock;
    }

    bool Refund::isLocked(uint32_t currentBlockHeight, uint32_t contractMinedInBlock) const {
        uint32_t unlockedIn = lockedUntil(contractMinedInBlock);

        return currentBlockHeight < unlockedIn;
    }

}
}
