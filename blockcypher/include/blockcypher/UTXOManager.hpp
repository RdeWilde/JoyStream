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

    static UTXOManager* createManager(WebSocketClient * wsclient, Coin::Network network);

    // adds address to set and sends tx_confirmation event on websocket
    bool addAddress(const Coin::P2PKHAddress & address);

    uint64_t balance() const { return _confirmedBalance; }
    uint64_t unconfirmedBalance() const { return _unconfirmedBalance; }

    bool refreshUtxoState(Client* restClient, const std::list<Coin::P2PKHAddress> &addresses);

    std::set<UTXO> getUtxoSet(uint64_t minValue, uint32_t minConfirmations = 0);
    void releaseUtxoSet(std::set<UTXO> utxos);

signals:

    void balanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

public slots:

private:

    UTXOManager(WebSocketClient* client, Coin::Network network);

    WebSocketClient* _wsClient;
    Coin::Network _network;

    std::set<UTXO> _unconfirmedUtxoSet;
    std::set<UTXO> _confirmedUtxoSet;
    std::set<UTXO> _lockedUtxoSet;

    void lockUtxoSet(std::set<UTXO> utxos);

    //maintain a set of addresses we are interested in
    std::set<QString> _addresses;

    bool addAddress(const QString & address);

    static std::vector<QString> batchAddresses(const std::set<QString> &addresses);

    std::vector<Address> fetchAddresses(Client * restClient, const std::vector<QString> &batches);

    // routine to handle TX payload from blockcypher
    TxResult processTx(const TX &tx);

    // routine to handle vector of TXRef from blockcypher
    std::vector<TxResult> processTxRefs(const std::vector<Address> &addresses);

    bool hasAddress(const QString &address);

    void updateUtxoSets(const TxResult & r);

    void updateBalances();

    // Sum of confirmed unspent outputs
    uint64_t _confirmedBalance;

    // Sum of all unspent outputs
    uint64_t _unconfirmedBalance;
};

}
#endif // BLOCKCYPHER_UTXOMANAGER_HPP
