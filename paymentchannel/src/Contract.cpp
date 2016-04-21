/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 20 2016
 */

#include <paymentchannel/Contract.hpp>
#include <common/Utilities.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <cmath>

namespace joystream {
namespace paymentchannel {

Contract::Contract()
    : _changeSet(false) {
}

Contract::Contract(const Coin::UnspentP2PKHOutput & funding,
                   const std::vector<Commitment> & commitments)
    : _funding(funding)
    , _commitments(commitments)
    , _changeSet(false) {
}

Contract::Contract(const Coin::UnspentP2PKHOutput & funding,
                   const std::vector<Commitment> & commitments,
                   const Coin::Payment & change)
    : _funding(funding)
    , _commitments(commitments)
    , _changeSet(true)
    , _change(change) {
}

uint Contract::addCommitment(const Commitment & commitment){

    // Add commitments
    _commitments.push_back(commitment);

    // Return number of commitments at present
    return _commitments.size();
}

void Contract::setChange(const Coin::Payment & change) {

    _change = change;
    _changeSet = true;
}

void Contract::clearChange() {
    _changeSet = false;
}

Coin::Transaction Contract::transaction() const {

    // Create transaction
    Coin::Transaction transaction;

    // Iterate channels and generate outputs
    for(const Commitment & c : _commitments)
        transaction.addOutput(c.contractOutput());

    // Add change output if set
    if(_changeSet)
        transaction.addOutput(_change.txOut());

    // Add (unsigned) input spending funding utxo
    transaction.addInput(Coin::TxIn(_funding.outPoint().getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

    // Creates spending input script
    setScriptSigToSpendP2PKH(transaction, 0, _funding.keyPair().sk());

    return transaction;
}

uint32_t Contract::transactionSize(uint32_t numberOfCommitments, bool hasChange) {

    return Coin::P2PKHScriptSig::maxLength() + // input
           Coin::P2SHScriptPubKey::length() * numberOfCommitments + // outputs
           (hasChange ? Coin::P2PKHScriptPubKey::length() : 0); // potential change
}

uint64_t Contract::fee(uint32_t numberOfCommitments, bool hasChange, quint64 feePerKb) {

    // Sizeof transaction
    quint64 txByteSize = transactionSize(numberOfCommitments, hasChange);

    // Seed on fee estimate at http://bitcoinfees.com/
    return ceil(feePerKb*((float)txByteSize/1000));
}

}
}
