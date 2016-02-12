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
#include <blockcypher/Client.hpp>
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

    UTXOManager(WebSocketClient* client);

    UTXOManager* createManager(WebSocketClient * wsclient,
                               const std::set<Coin::P2PKHAddress> &addresses,
                               Client * restClient);

    // adds address to set and sends tx_confirmation event on websocket
    bool addAddress(const Coin::P2PKHAddress & address);

    uint64_t balance() const { return _balance; }
    uint64_t unconfirmedBalance() const { return _balance_zero_conf; }

signals:

    void balanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

public slots:

private:
    WebSocketClient* _wsClient;

    std::set<UTXORef> _unconfirmedUtxoSet;
    std::set<UTXORef> _confirmedUtxoSet;

    //maintain a set of addresses we are interested in
    std::set<QString> _addresses;

    // routine to handle TX payload from blockcypher
    void processTx(const TX &tx);

    // routine to handle TXRef from blockcypher
    void processTxRef(const std::vector<TXRef> &txrefs);

    bool hasAddress(const QString &address);

    void updateBalances(bool notify = false);

    uint64_t _balance; // confirmed balance
    uint64_t _balance_zero_conf; // 0-conf balance
};

}
#endif // BLOCKCYPHER_UTXOMANAGER_HPP
