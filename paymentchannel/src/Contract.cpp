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

Contract::Contract()
    : _changeSet(false) {
}

Contract::Contract(const Coin::UnspentOutputSet & funding)
    : _funding(funding)
    , _changeSet(false) {
}

Contract::Contract(const Coin::UnspentOutputSet &funding,
                   const std::vector<Commitment> & commitments)
    : _funding(funding)
    , _commitments(commitments)
    , _changeSet(false) {
}

Contract::Contract(const Coin::UnspentOutputSet & funding,
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

    _funding.finance(transaction, Coin::SigHashType::standard());

    return transaction;
}

uint32_t Contract::transactionSize(uint32_t numberOfCommitments, bool hasChange, int numberOfInputs) {

    // Create blank contract with correct structure
    Coin::UnspentOutputSet funding;

    // NB: We must have valid private key, as there will be actual signing with key
    for(int i = 0; i < numberOfInputs; i++) {
        auto utxo = std::make_shared<Coin::UnspentP2PKHOutput>(Coin::PrivateKey("153213303DA61F20BD67FC233AA33262"), Coin::typesafeOutPoint(), 0);
        funding.insert(utxo);
    }

    Contract c(Coin::UnspentOutputSet({funding}));

    for(uint32_t i = 0;i < numberOfCommitments; i++)
        c.addCommitment(Commitment());

    if(hasChange)
        c.setChange(Coin::Payment(0, Coin::PrivateKey("153213303DA61F20BD67FC233AA33262").toPublicKey().toPubKeyHash()));

    // Generate transaction
    Coin::Transaction tx = c.transaction();

    // return total size
    return tx.getSize();
}

uint64_t Contract::fee(uint32_t numberOfCommitments, bool hasChange, quint64 feePerKb, int numberOfInputs) {

    // Sizeof transaction
    quint64 txByteSize = transactionSize(numberOfCommitments, hasChange, numberOfInputs);

    // Seed on fee estimate at http://bitcoinfees.com/
    return ceil(feePerKb*((float)txByteSize/1024));
}

}
}
