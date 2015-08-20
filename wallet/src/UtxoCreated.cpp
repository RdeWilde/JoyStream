/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#include <wallet/UtxoCreated.hpp>

namespace Wallet {

UtxoCreated::UtxoCreated(const Coin::typesafeOutPoint & outPointCreated, const Coin::TxOut & newUtxo, const QDateTime & seen)
    : _outPointCreated(outPointCreated)
    , _newUtxo(newUtxo)
    , _seen(seen) {
}

Coin::typesafeOutPoint UtxoCreated::outPointCreated() const {
    return _outPointCreated;
}

void UtxoCreated::setOutPointCreated(const Coin::typesafeOutPoint & outPointCreated) {
    _outPointCreated = outPointCreated;
}

Coin::TxOut UtxoCreated::newUtxo() const {
    return _newUtxo;
}

void UtxoCreated::setNewUtxo(const Coin::TxOut & newUtxo) {
    _newUtxo = newUtxo;
}

QDateTime UtxoCreated::seen() const {
    return _seen;
}

void UtxoCreated::setSeen(const QDateTime & seen) {
    _seen = seen;
}

}
