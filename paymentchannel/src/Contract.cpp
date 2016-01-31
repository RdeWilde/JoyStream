/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Contract.hpp>
#include <common/Utilities.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

namespace joystream {
namespace paymentchannel {

    Contract::Contract(const Coin::UnspentP2PKHOutput & funding,
                       const std::vector<Commitment> & commitments,
                       const Coin::Payment & change)
        : _funding(funding)
        , _commitments(commitments)
        , _change(change) {
    }

    /**
    bool Contract::validateContractTrasaction(const Coin::Transaction & transaction) {

        // Do some magic
        return true;
    }
    */

    uint Contract::addCommitments(const Commitment & commitment){

        // Add commitments
        _commitments.push_back(commitment);

        // Return number of commitments at present
        return _commitments.size();
    }

    Coin::Transaction Contract::transaction() const {

        // Create transaction
        Coin::Transaction transaction;

        // Iterate channels and generate outputs
        for(int i = 0;i < _commitments.size();i++)
            transaction.addOutput(_commitments[i].contractOutput());

        // Add change output
        transaction.addOutput(_change.txOut());

        // Add (unsigned) input spending funding utxo
        transaction.addInput(Coin::TxIn(_funding.outPoint().getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

        // Creates spending input script
        setScriptSigToSpendP2PKH(transaction, 0, _funding.keyPair().sk());

        return transaction;
    }

}
}
