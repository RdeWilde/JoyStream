/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#ifndef BLOCKCYPHER_UTXOMANAGER_HPP
#define BLOCKCYPHER_UTXOMANAGER_HPP

#include <set>

#include <common/typesafeOutPoint.hpp>
#include <common/P2PKHAddress.hpp>

#include <blockcypher/WebSocketClient.hpp>
#include <blockcypher/TX.hpp>
#include <blockcypher/TXRef.hpp>
#include <blockcypher/UTXORef.hpp>
#include <blockcypher/TransactionRelevantUtxo.hpp>

#include <QObject>

namespace BlockCypher {

typedef std::map<Coin::TransactionId, TransactionRelevantUtxo> txToUtxoMap_t;

class UTXOManager : public QObject
{
    Q_OBJECT

public:

    UTXOManager(WebSocketClient* client, const std::set<Coin::P2PKHAddress> &addresses);

    static void InitialiseUtxo(const std::set<QString> & addresses, txToUtxoMap_t & txUtxoMap);

    // adds address to set and sends tx_confirmation event on websocket
    bool addAddress(const Coin::P2PKHAddress & address);

signals:

public slots:

private:
    WebSocketClient* _wsclient;

    //all transactions which create and destroy UTXOs for addresses we control
    txToUtxoMap_t _txUtxoMap;

    //maintain a set of addresses we are interested in
    std::set<QString> _addresses;

    // routine to handle TX payload from blockcypher
    void onTransaction(const TX &tx);

    // routine to handle TXRef from blockcypher
    void onTxRef(const TXRef &txref);

    bool hasAddress(const QString &address);

    void updateBalances();

    uint64_t _balance; // confirmed balance
    uint64_t _balance_zero_conf; // 0-conf balance
};

}
#endif // BLOCKCYPHER_UTXOMANAGER_HPP
