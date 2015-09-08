/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Settlement.hpp>
#include <common/P2PKHScriptPubKey.hpp>

#include <CoinCore/CoinNodeData.h> // TxOut

Settlement::Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                       const Commitment & commitment,
                       const Coin::Payment & toPayor,
                       const Coin::Payment & toPayee)
    : Termination(contractOutPoint, commitment, toPayor)
    , _toPayee(toPayee) {
}

Coin::Transaction Settlement::unSignedTransaction() const {

    // Build refund transaction
    Coin::Transaction tx = Termination::unSignedTransaction();

    // Payee output
    tx.addOutput(_toPayee.txOut());

    return tx;
}

int64_t Settlement::fee() const {
    return _commitment.value() - (_toPayor.value() + _toPayee.value());
}
