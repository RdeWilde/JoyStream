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
#include <common/P2SHScriptSig.hpp>

Refund::Refund(const Coin::typesafeOutPoint & contractOutPoint,
               const Commitment & commitment,
               const Coin::Payment & toPayor,
               uint32_t lockTime)
    : Termination(contractOutPoint, commitment, toPayor)
    , _lockTime(lockTime) {
}

Coin::Transaction Refund::unSignedTransaction() const {

    // Build refund transaction
    Coin::Transaction tx = Termination::unSignedTransaction();

    // Set lock time
    tx.lockTime = _lockTime;

    return tx;
}

int64_t Refund::fee() const {
    return _commitment.value() - _toPayor.value();
}
