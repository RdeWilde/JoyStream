/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UNSPENT_OUTPUT_HPP
#define COIN_UNSPENT_OUTPUT_HPP

#include <common/typesafeOutPoint.hpp>
#include <common/SigHashType.hpp>

#include <CoinCore/typedefs.h>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint


namespace Coin {

class UnspentOutput {
public:

    // Default constructor
    UnspentOutput();

    // Constructor from members
    UnspentOutput(const typesafeOutPoint & outPoint, uint64_t setValue);

    // Comparator
    bool operator==(const UnspentOutput & o) const;
    bool operator!=(const UnspentOutput & o) const;

    // Getters and setters
    typesafeOutPoint outPoint() const;
    void setOutPoint(const typesafeOutPoint & outPoint);

    uint64_t value() const;
    void setValue(quint64 value);

    virtual uchar_vector scriptPubKey() const = 0;
    virtual uchar_vector scriptSig(const Transaction & tx, const SigHashType & sigHashType) const = 0;

private:

    // OutPoint locating utxo
    typesafeOutPoint _outPoint;

    // Value of output
    uint64_t _value;
};

}

#endif // COIN_UNSPENT_OUTPUT_HPP
