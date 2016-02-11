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

class UTXOManager : public QObject
{
    Q_OBJECT

public:

    UTXOManager(WebSocketClient* client, const std::set<Coin::P2PKHAddress> &addresses);

    static void InitialiseUtxo(const std::set<QString> & addresses,
                               std::set<UTXORef> &confirmedSet, std::set<UTXORef> &unconfirmedSet);

    // adds address to set and sends tx_confirmation event on websocket
    bool addAddress(const Coin::P2PKHAddress & address);

signals:

public slots:

private:
    WebSocketClient* _wsclient;

    std::set<UTXORef> _unconfirmed_utxo_set;
    std::set<UTXORef> _confirmed_utxo_set;

    //maintain a set of addresses we are interested in
    std::set<QString> _addresses;

    // routine to handle TX payload from blockcypher
    void processTx(const TX &tx);

    // routine to handle TXRef from blockcypher
    void processTxRef(const TXRef &txref);

    bool hasAddress(const QString &address);

    void updateBalances();

    uint64_t _balance; // confirmed balance
    uint64_t _balance_zero_conf; // 0-conf balance
};

}
#endif // BLOCKCYPHER_UTXOMANAGER_HPP
