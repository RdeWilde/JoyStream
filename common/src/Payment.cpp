/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#include <common/Payment.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::TxOut

namespace Coin {

Payment::Payment()
    : _value(0) {
}

Payment::Payment(int64_t value, const Coin::PubKeyHash & destination)
    : _value(value)
    , _destination(destination){

}

Coin::TxOut Payment::txOut() const {
    return Coin::TxOut(_value, Coin::P2PKHScriptPubKey(_destination).serialize());
}

int64_t Payment::value() const {
    return _value;
}

void Payment::setValue(int64_t value) {
    _value = value;
}

Coin::PubKeyHash Payment::destination() const {
    return _destination;
}

void Payment::setDestination(const Coin::PubKeyHash & destination) {
    _destination = destination;
}



}
