/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 20 2016
 */

#include <paymentchannel/Contract.hpp>
#include <paymentchannel/Commitment.hpp>
#include <common/Payment.hpp>
#include <common/Utilities.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/UnspentOutputSet.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <cmath>

namespace joystream {
namespace paymentchannel {

Contract::Contract() {
}

boost::optional<Coin::UnspentOutputSet> Contract::funding() const {
    return _funding;
}

void Contract::setFunding(const boost::optional<Coin::UnspentOutputSet> &funding) {
    _funding = funding;
}

Contract::Commitments Contract::commitments() const {
    return _commitments;
}

void Contract::setCommitments(const Commitments &commitments) {
    _commitments = commitments;
}

boost::optional<Coin::Payment> Contract::change() const {
    return _change;
}

void Contract::setChange(const boost::optional<Coin::Payment> &change) {
    _change = change;
}

Coin::Transaction Contract::transaction() const {

    // Create transaction
    Coin::Transaction transaction;

    // Iterate channels and generate outputs
    for(const Commitment & c : _commitments)
        transaction.addOutput(c.contractOutput());

    // Add change output if set
    if(_change.is_initialized())
        transaction.addOutput(_change.get().txOut());

    // Add funding if set
    if(_funding.is_initialized())
        _funding.get().finance(transaction, Coin::SigHashType::standard());

    return transaction;
}

uint64_t Contract::totalFee(uint32_t numberOfCommitments, bool hasChange, quint64 feePerKb, uint64_t sizeOfAllInputs) {

    // Sizeof transaction
    quint64 txByteSize = transactionSize(numberOfCommitments, hasChange) + sizeOfAllInputs;

    // Seed on fee estimate at http://bitcoinfees.com/
    return ceil(feePerKb*((float)txByteSize/1024));
}

uint64_t Contract::transactionSize(uint32_t numberOfCommitments, bool hasChange) {

    Contract c;

    // set commitments
    c.setCommitments(Commitments(numberOfCommitments));

    // set change
    if(hasChange)
        c.setChange(Coin::Payment(0, Coin::PrivateKey("153213303DA61F20BD67FC233AA33262").toPublicKey().toPubKeyHash()));

    // Generate transaction
    Coin::Transaction tx = c.transaction();

    // return total size
    return tx.getSize();
}

}
}
