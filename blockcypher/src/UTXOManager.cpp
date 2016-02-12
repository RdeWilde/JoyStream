/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <blockcypher/UTXOManager.hpp>
#include <blockcypher/Address.hpp>

namespace BlockCypher {

    UTXOManager::UTXOManager(WebSocketClient * wsclient)
        : _wsClient(wsclient),
          _balance(0),
          _balance_zero_conf(0)
    {

        // connect signals from websocket client to our private slots
        QObject::connect(_wsClient, &WebSocketClient::txArrived, [this](const TX & tx){
            TxResult r = processTx(tx);
            updateUtxoSets(r);
            updateBalances(true);
        });

    }

    UTXOManager* UTXOManager::createManager(WebSocketClient * wsclient,
                                     const std::set<Coin::P2PKHAddress> &addresses,
                                     Client * restClient)
    {

        UTXOManager* manager = new UTXOManager(wsclient);

        if(!manager) throw std::runtime_error("unable to create a UTXO Manager");

        // If REST api client is provided we can initialise the utxo state
        if(restClient) {
            std::vector<TXRef> txrefs = manager->fetchTxRefs(restClient, addresses);
            std::vector<TxResult> results = manager->processTxRefs(txrefs);
            for(auto & r : results) manager->updateUtxoSets(r);
            manager->updateBalances(false);
        }

        // add event filter after initialization
        for(const Coin::P2PKHAddress & address : addresses) {
            manager->addAddress(address);
        }

        return manager;
    }

    std::vector<QString> UTXOManager::batchAddresses(const std::set<Coin::P2PKHAddress> & p2pkhAddresses) {
        std::set<QString> addresses;

        for(const Coin::P2PKHAddress & address : p2pkhAddresses) {
            addresses.insert(address.toBase58CheckEncoding());
        }

        return batchAddresses(addresses);
    }

    std::vector<QString> UTXOManager::batchAddresses(const std::set<QString> &addresses) {
        // Create a vector of batches of addresses (QString of semicolon separated list)
        // The maximum number of elements that can be batched in a single call is 100.
        std::vector<QString> batches;
        uint counter = 0;
        uint batchNo;
        for(const QString &addr : addresses) {
            batchNo =  counter / 100;
            if(batches.empty() || batches.size() < batchNo) {
                batches.push_back(addr);
            } else {
                batches[batchNo] += ";" + addr;
            }
            counter++;
        }

        return batches;
    }

    std::vector<TXRef> UTXOManager::fetchTxRefs(Client * restClient, const std::vector<QString> & batches) {
        std::vector<TXRef> txrefs;

        for(const QString & batch : batches) {
            Address response = restClient->addressEndPoint(batch);
            txrefs.insert(txrefs.end(), response._txrefs.begin(), response._txrefs.end());
            txrefs.insert(txrefs.end(), response._unconfirmed_txrefs.begin(), response._unconfirmed_txrefs.end());
        }

        return txrefs;
    }

    std::vector<TXRef> UTXOManager::fetchTxRefs(Client * restClient, const std::set<Coin::P2PKHAddress> & addresses) {
        return fetchTxRefs(restClient, batchAddresses(addresses));
    }

    std::vector<TXRef> UTXOManager::fetchTxRefs(Client * restClient, const std::set<QString> & addresses) {
        return fetchTxRefs(restClient, batchAddresses(addresses));
    }

    void UTXOManager::refreshUtxoState(Client* restClient) {
        //use a mutex or bool flag to stop updates from wsclient while we reset the state?
        std::vector<TXRef> txrefs = fetchTxRefs(restClient, _addresses);
        std::vector<TxResult> results = processTxRefs(txrefs);
        _unconfirmedUtxoSet.clear();
        _confirmedUtxoSet.clear();

        for(auto & r : results) updateUtxoSets(r);

        updateBalances(true);
    }

    std::vector<TxResult> UTXOManager::processTxRefs(const std::vector<TXRef> &txrefs) {
        std::vector<TxResult> results;

        for(const TXRef &t : txrefs) {
            if(t._tx_output_n >= 0 && t._spent == false) {
                Coin::typesafeOutPoint outpoint(t._tx_hash, t._tx_output_n);
                UTXO utxo(QString::fromStdString(t._addressString), outpoint, t._value);
                TxResult result;
                result.confirmations(t._confirmations);
                result.creates(utxo);
                results.push_back(result);
            }
        }

        return results;
    }

    bool UTXOManager::addAddress(const Coin::P2PKHAddress & address) {
        QString addr = address.toBase58CheckEncoding();
        //return false if address already exists
        if(hasAddress(addr)) return false;

        //insert and add tx_confirmation event to websocket filters
        _addresses.insert(addr);
        _wsClient->addEvent(Event::makeTxConfirmation(2, addr));
        return true;
    }

    TxResult UTXOManager::processTx(const TX & tx) {
        TxResult result;

        Coin::TransactionId txid = Coin::TransactionId::fromRPCByteOrder(tx.hash().toStdString());

        if(tx.double_spend()) return result;

        result.confirmations(tx.confirmations());

        for(const TXInput & input : tx.inputs()) {
            if(input.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            // We expect only one address in output.addresses() array
            QString addr = input.addresses()[0];

            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(Coin::TransactionId::fromRPCByteOrder(input.prev_hash().toStdString()), input.index());
            UTXO utxo(addr, outpoint, input.value());

            result.destroys(utxo);
        }

        int32_t index = -1;
        for(const TXOutput & output : tx.outputs()) {
            index++;
            if(output.spent_by()) continue;

            if(output.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            //we expect only one address in output.addresses() array
            QString addr = output.addresses()[0];
            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(txid, index);
            UTXO utxo(addr, outpoint, output.value());

            result.creates(utxo);
        }

        return result;
    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateUtxoSets(const TxResult & r) {
        for(const UTXO &utxo : r.creates()) {
            if(r.confirmations() > 0) {
                _confirmedUtxoSet.insert(utxo);
            } else {
                _unconfirmedUtxoSet.insert(utxo);
            }
        }

        for(const UTXO &utxo: r.destroys()) {
            _confirmedUtxoSet.erase(utxo);
            _unconfirmedUtxoSet.erase(utxo);
        }
    }

    void UTXOManager::updateBalances(bool notify) {

        uint64_t confirmedBalance = 0;

        // Calculate new confirmed balance
        for(const UTXO & utxo: _confirmedUtxoSet) {
            confirmedBalance += utxo.value();
        }

        // Calculate new unconfirmed balance
        uint64_t unconfirmedBalance = confirmedBalance;
        for(const UTXO & utxo: _unconfirmedUtxoSet) {
            unconfirmedBalance += utxo.value();
        }

        //if changed emit signals
        if(_balance != confirmedBalance || _balance_zero_conf != unconfirmedBalance) {
            _balance = confirmedBalance;
            _balance_zero_conf = unconfirmedBalance;
            if(notify) emit balanceChanged(confirmedBalance, unconfirmedBalance);
        }

    }
}
