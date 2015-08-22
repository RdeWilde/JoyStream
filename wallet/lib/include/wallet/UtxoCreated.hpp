/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2015
 */

#ifndef WALLET_UTXO_CREATED_HPP
#define WALLET_UTXO_CREATED_HPP

#include <common/UnspentP2PKHOutput.hpp>

#include <QDateTime>

namespace Wallet {

    class UtxoCreated {

    public:

        UtxoCreated(const Coin::UnspentP2PKHOutput & utxo,
                    const QDateTime & seen);

        Coin::UnspentP2PKHOutput utxo() const;
        void setUtxo(const Coin::UnspentP2PKHOutput & utxo);

        QDateTime seen() const;
        void setSeen(const QDateTime & seen);

    private:

        Coin::UnspentP2PKHOutput _utxo;

        // When transation was first seen
        QDateTime _seen;


    };

}

#endif // WALLET_UTXO_CREATED_HPP

