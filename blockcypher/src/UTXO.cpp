/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <blockcypher/UTXO.hpp>

namespace blockcypher {

bool UTXO::operator==(const UTXO & o) const {

    // block height is not used in equality operator
    // because its not relevent when inserting and erasing
    // a utxo from a set
    return _value == o.value() &&
           _outpoint == o.outPoint() &&
           _address == o.address();
}

bool UTXO::operator!=(const UTXO & o) const {
    return !(*this == o);
}

bool UTXO::operator<(const UTXO & o) const {
    if(_value == o.value()) {
        if(_outpoint == o.outPoint()) {
            return _address < o.address();
        } else {
            return _outpoint < o.outPoint();
        }
    } else {
        // utxo are sorted by descending value.
        // to minimize the number of inputs required to meet a minimum value requirement.
        return _value > o.value();
    }
}

}
