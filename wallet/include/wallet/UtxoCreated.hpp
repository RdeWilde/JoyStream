/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#ifndef WALLET_UTXO_CREATED_HPP
#define WALLET_UTXO_CREATED_HPP

#include <common/typesafeOutPoint.hpp>
#include <CoinCore/CoinNodeData.h> // Output

#include <QDateTime>

namespace Wallet {

    class UtxoCreated {

    public:

        UtxoCreated(const Coin::typesafeOutPoint & outPointCreated, const Coin::TxOut & newUtxo, const QDateTime & seen);

        // Getters and setters
        Coin::typesafeOutPoint outPointCreated() const;
        void setOutPointCreated(const Coin::typesafeOutPoint &outPointCreated);

        Coin::TxOut newUtxo() const;
        void setNewUtxo(const Coin::TxOut &newUtxo);

        QDateTime seen() const;
        void setSeen(const QDateTime &seen);

    private:

        //
        Coin::typesafeOutPoint _outPointCreated;
        Coin::TxOut _newUtxo;

        // When transation was first seen
        QDateTime _seen;
    };

}

#endif // WALLET_UTXO_CREATED_HPP

