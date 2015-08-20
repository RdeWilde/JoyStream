/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#include <wallet/UtxoDestroyed.hpp>

namespace Wallet {

UtxoDestroyed::UtxoDestroyed(const Coin::TransactionId & spendingTx,
                              const QDateTime & seen,
                              const Coin::typesafeOutPoint outPointDestroyed,
                              const Coin::TxOut & oldUtxo)
    : _spendingTx(spendingTx)
    , _seen(seen)
    , _outPointDestroyed(outPointDestroyed)
    , _oldUtxo(oldUtxo) {
}

Coin::TransactionId UtxoDestroyed::spendingTx() const {
    return _spendingTx;
}

void UtxoDestroyed::setSpendingTx(const Coin::TransactionId & spendingTx) {
    _spendingTx = spendingTx;
}

QDateTime UtxoDestroyed::seen() const {
    return _seen;
}

void UtxoDestroyed::setSeen(const QDateTime & seen) {
    _seen = seen;
}

Coin::typesafeOutPoint UtxoDestroyed::outPointDestroyed() const {
    return _outPointDestroyed;
}

void UtxoDestroyed::setOutPointDestroyed(const Coin::typesafeOutPoint & outPointDestroyed) {
    _outPointDestroyed = outPointDestroyed;
}

Coin::TxOut UtxoDestroyed::oldUtxo() const {
    return _oldUtxo;
}

void UtxoDestroyed::setOldUtxo(const Coin::TxOut & oldUtxo) {
    _oldUtxo = oldUtxo;
}

}
