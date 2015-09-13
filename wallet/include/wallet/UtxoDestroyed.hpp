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
                      quint32 indexOfInputInSpendingTx,
                      const Coin::TxIn & inputInSpendingTx,
                      const Coin::TxOut & oldUtxo);

        Coin::TransactionId spendingTx() const;
        void setSpendingTx(const Coin::TransactionId &spendingTx);

        QDateTime seen() const;
        void setSeen(const QDateTime & seen);

        quint32 indexOfInputInSpendingTx() const;
        void setIndexOfInputInSpendingTx(quint32 indexOfInputInSpendingTx);

        Coin::TxIn inputInSpendingTx() const;
        void setInputInSpendingTx(const Coin::TxIn & inputInSpendingTx);

        Coin::TxOut oldUtxo() const;
        void setOldUtxo(const Coin::TxOut & oldUtxo);

    private:

        // Transaction spending output
        Coin::TransactionId _spendingTx;

        // When spending transation was first seen
        QDateTime _seen;

        // Index of spening input in the spending transaction
        quint32 _indexOfInputInSpendingTx;

        // The actual input doing the spending
        Coin::TxIn _inputInSpendingTx;

        // Output being spent (in previous transaction)
        Coin::TxOut _oldUtxo;
    };
}

#endif // WALLET_UTXO_DESTROYED_HPP

