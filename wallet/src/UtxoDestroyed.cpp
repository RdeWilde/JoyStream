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
                             quint32 indexOfInputInSpendingTx,
                             const Coin::TxIn & inputInSpendingTx,
                             const Coin::TxOut & oldUtxo)
    : _spendingTx(spendingTx)
    , _seen(seen)
    , _indexOfInputInSpendingTx(indexOfInputInSpendingTx)
    , _inputInSpendingTx(inputInSpendingTx)
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

quint32 UtxoDestroyed::indexOfInputInSpendingTx() const {
    return _indexOfInputInSpendingTx;
}

void UtxoDestroyed::setIndexOfInputInSpendingTx(quint32 indexOfInputInSpendingTx) {
    _indexOfInputInSpendingTx = indexOfInputInSpendingTx;
}

Coin::TxIn UtxoDestroyed::inputInSpendingTx() const {
    return _inputInSpendingTx;
}

void UtxoDestroyed::setInputInSpendingTx(const Coin::TxIn & inputInSpendingTx) {
    _inputInSpendingTx = inputInSpendingTx;
}

Coin::TxOut UtxoDestroyed::oldUtxo() const {
    return _oldUtxo;
}

void UtxoDestroyed::setOldUtxo(const Coin::TxOut & oldUtxo) {
    _oldUtxo = oldUtxo;
}

}
