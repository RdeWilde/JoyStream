/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#ifndef BLOCKCYPHER_TRANSACTION_RELEVANT_UTXO_H
#define BLOCKCYPHER_TRANSACTION_RELEVANT_UTXO_H

#include <blockcypher/UTXORef.hpp>

#include <set>

namespace BlockCypher {

class TransactionRelevantUtxo {

public:

    void confirmations(const uint32_t & conf) { _confirmations = conf; }
    uint32_t confirmations() const { return _confirmations; }

    void creates(const UTXORef & utxo)  { _creationSet.insert(utxo); }
    std::set<UTXORef> creates() const { return _creationSet; }

    void destroys(const UTXORef & utxo) { _destructionSet.insert(utxo); }
    std::set<UTXORef> destroys() const { return _destructionSet; }

private:
    uint32_t _confirmations;
    std::set<UTXORef> _creationSet;
    std::set<UTXORef> _destructionSet;
};

}

#endif // BLOCKCYPHER_TRANSACTION_RELEVANT_UTXO_H
