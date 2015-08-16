/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <core/extension/PaymentChannel/Contract.hpp>
#include <CoinCore/StandardTransactions.h>

#include <QVector>

Contract::Contract(const Coin::UnspentP2PKHOutput & funding,
                   const QVector<Coin::Commitment> & channelCommitments,
                   const Coin::Payment & change)
    : _funding(funding)
    , _channelCommitments(channelCommitments)
    , _change(change) {
}

bool Contract::validateContractTrasaction(const Coin::Transaction & transaction) {

    // Do some magic
    return true;
}

Coin::Transaction Contract::transaction() const {

    // Create transaction
    Coin::Transaction transaction;

    // Create input spending utxo
    Coin::typesafeOutPoint outPoint = _funding.outPoint();

    Coin::P2AddressTxIn txIn(outPoint.transactionId().toUCharVector(),
                             outPoint.index(),
                             _funding.keyPair().pk().toUCharVector());

    // Add input
    transaction.addInput(txIn);

    // Add all outputs
    for(QVector<Coin::Commitment>::const_iterator
        i = _channelCommitments.constBegin(),
        end = _channelCommitments.constEnd();
        i != end;
        i++) {

        // Get commitment
        Coin::Commitment c = *i;

        // Create corresponding contract transaction output
        Coin::StandardTxOut txOut = c.toTxOut();

        // Add to transaction
        transaction.addOutput(txOut);
    }

    // Add refund
    Coin::StandardTxOut refundOutput;
    //transaction.addOutput(refundOutput);

    // Sign!
    //?

    return transaction;
}

/**
Coin::OLD_P2SHTxOut Contract::toP2SHTxOut() {

}
*/
