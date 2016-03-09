/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#ifndef BLOCKCYPHER_UTXO_H
#define BLOCKCYPHER_UTXO_H

#include <common/typesafeOutPoint.hpp>
#include <common/P2PKHAddress.hpp>

namespace BlockCypher {

class UTXO {

public:
    UTXO(const QString & addr, const Coin::typesafeOutPoint & outpoint, uint64_t value, int32_t height = -1)
        : _address(addr),
          _outpoint(outpoint),
          _value(value),
          _height(height)
    {}

    UTXO() {}

    uint64_t value() const { return _value; }
    Coin::typesafeOutPoint outPoint() const { return _outpoint; }
    QString address() const { return _address; }
    bool confirmed() const { return _height > 0; }
    int32_t height() const { return _height; }

    // Comparators
    bool operator==(const UTXO & o) const;
    bool operator!=(const UTXO & o) const;
    bool operator<(const UTXO & o) const;

private:
    QString _address;
    Coin::typesafeOutPoint _outpoint;
    uint64_t _value;

    // block height for a confirmed utxo, -1 if unconfirmed
    mutable int32_t _height; // not used in comparator
};

}
#endif // BLOCKCYPHER_UTXO_H
