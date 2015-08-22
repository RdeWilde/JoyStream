/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#include <wallet/UtxoCreated.hpp>

namespace Wallet {

UtxoCreated::UtxoCreated(const Coin::UnspentP2PKHOutput & utxo, const QDateTime & seen)
    : _utxo(utxo)
    , _seen(seen) {
}

Coin::UnspentP2PKHOutput UtxoCreated::utxo() const {
    return _utxo;
}

void UtxoCreated::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
    _utxo = utxo;
}

QDateTime UtxoCreated::seen() const {
    return _seen;
}

void UtxoCreated::setSeen(const QDateTime & seen) {
    _seen = seen;
}

}
