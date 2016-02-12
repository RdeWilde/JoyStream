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
#include <blockcypher/UTXO.hpp>
#include <blockcypher/TxResult.hpp>

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

    void refreshUtxoState(Client* restClient);

signals:

    void balanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

public slots:

private:
    WebSocketClient* _wsClient;

    std::set<UTXO> _unconfirmedUtxoSet;
    std::set<UTXO> _confirmedUtxoSet;

    //maintain a set of addresses we are interested in
    std::set<QString> _addresses;

    static std::vector<QString> batchAddresses(const std::set<Coin::P2PKHAddress> & p2pkhAddresses);
    static std::vector<QString> batchAddresses(const std::set<QString> &addresses);

    std::vector<TXRef> fetchTxRefs(Client * restClient, const std::vector<QString> &batches);
    std::vector<TXRef> fetchTxRefs(Client * restClient, const std::set<Coin::P2PKHAddress> & addresses);
    std::vector<TXRef> fetchTxRefs(Client * restClient, const std::set<QString> & addresses);

    // routine to handle TX payload from blockcypher
    TxResult processTx(const TX &tx);

    // routine to handle vector of TXRef from blockcypher
    std::vector<TxResult> processTxRefs(const std::vector<TXRef> &txrefs);

    bool hasAddress(const QString &address);

    void updateUtxoSets(const TxResult & r);

    void updateBalances(bool notify = false);

    uint64_t _balance; // confirmed balance
    uint64_t _balance_zero_conf; // 0-conf balance
};

}
#endif // BLOCKCYPHER_UTXOMANAGER_HPP
