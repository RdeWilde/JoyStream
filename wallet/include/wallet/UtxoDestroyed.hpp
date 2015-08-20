/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#ifndef WALLET_UTXO_DESTROYED_HPP
#define WALLET_UTXO_DESTROYED_HPP

#include <common/TransactionId.hpp>
#include <common/typesafeOutPoint.hpp>
#include <CoinCore/CoinNodeData.h> // Output

#include <QDateTime>

namespace Wallet {

    class UtxoDestroyed {

    public:

        UtxoDestroyed(const Coin::TransactionId & spendingTx,
                      const QDateTime & seen,
                      const Coin::typesafeOutPoint outPointDestroyed,
                      const Coin::TxOut & oldUtxo);

        // Getters and setters
        Coin::TransactionId spendingTx() const;
        void setSpendingTx(const Coin::TransactionId & spendingTx);

        QDateTime seen() const;
        void setSeen(const QDateTime & seen);

        Coin::typesafeOutPoint outPointDestroyed() const;
        void setOutPointDestroyed(const Coin::typesafeOutPoint & outPointDestroyed);

        Coin::TxOut oldUtxo() const;
        void setOldUtxo(const Coin::TxOut & oldUtxo);

    private:

        // Transaction spending output
        Coin::TransactionId _spendingTx;

        // When transation was first seen
        QDateTime _seen;

        // Outpoint and output destroyed
        Coin::typesafeOutPoint _outPointDestroyed;
        Coin::TxOut _oldUtxo;
    };
}

#endif // WALLET_UTXO_DESTROYED_HPP

