/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#ifndef BLOCKCYPHER_TXRESULT_H
#define BLOCKCYPHER_TXRESULT_H

#include <blockcypher/UTXO.hpp>

#include <set>

namespace blockcypher {

class TxResult {

public:

    void confirmations(const uint32_t & conf) { _confirmations = conf; }
    uint32_t confirmations() const { return _confirmations; }

    void creates(const UTXO & utxo)  { _creationSet.insert(utxo); }
    std::set<UTXO> creates() const { return _creationSet; }

    void destroys(const UTXO & utxo) { _destructionSet.insert(utxo); }
    std::set<UTXO> destroys() const { return _destructionSet; }

private:
    uint32_t _confirmations;
    std::set<UTXO> _creationSet;
    std::set<UTXO> _destructionSet;
};

}

#endif // BLOCKCYPHER_TXRESULT_H
