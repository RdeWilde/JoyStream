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
        // which spends it will have nLockTime set to the commitment locktime

        return Coin::UnspentP2SHOutput(_payorContractKeyPair,
                                       _commitment.redeemScript().serialized(),
                                       uchar_vector(0x00), /*OP_FALSE*/
                                       _contractOutPoint,
                                       _commitment.value());
    }

    uint32_t Refund::lockedUntil() const {
        return _commitment.lockTime();
    }

    bool Refund::isLocked(uint32_t currentBlockHeight) const {
        uint32_t locktime = lockedUntil();

        if(locktime < uint32_t(500000000)){
            return locktime >= currentBlockHeight;
        } else {
            return locktime > std::time(nullptr);
        }
    }

}
}
