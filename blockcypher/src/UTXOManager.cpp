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
            processTx(tx);
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
            // Create a vector of batches of addresses (QString of semicolon separated list)
            // The maximum number of elements that can be batched in a single call is 100.
            std::vector<QString> batches;

            uint counter = 0;
            uint batchNo;
            for(const Coin::P2PKHAddress &addr : addresses) {
                batchNo =  counter / 100;
                if(batches.empty() || batches.size() < batchNo) {
                    batches.push_back(addr.toBase58CheckEncoding());
                } else {
                    batches[batchNo] += ";" + addr.toBase58CheckEncoding();
                }
                counter++;
            }

            for(const QString & batch : batches) {
                Address response = restClient->addressEndPoint(batch);

                manager->processTxRef(response._txrefs);
                manager->processTxRef(response._unconfirmed_txrefs);
            }

            manager->updateBalances();
        }

        for(const Coin::P2PKHAddress & address : addresses) {
            manager->addAddress(address);
        }

        return manager;
    }

    void UTXOManager::processTxRef(const std::vector<TXRef> &txrefs) {
        for(const TXRef &t : txrefs) {
            if(t._tx_output_n >= 0 && t._spent == false) {

                Coin::typesafeOutPoint outpoint(t._tx_hash, t._tx_output_n);
                UTXORef utxo(QString::fromStdString(t._addressString), outpoint, t._value);

                if(t._confirmations > 0) {
                    _confirmedUtxoSet.insert(utxo);
                } else {
                    _unconfirmedUtxoSet.insert(utxo);
                }
            }
        }
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

    void UTXOManager::processTx(const TX & tx) {

        Coin::TransactionId txid = Coin::TransactionId::fromRPCByteOrder(tx.hash().toStdString());

        if(tx.double_spend()) return;

        for(const TXInput & input : tx.inputs()) {
            if(input.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            // We expect only one address in output.addresses() array
            QString addr = input.addresses()[0];

            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(Coin::TransactionId::fromRPCByteOrder(input.prev_hash().toStdString()), input.index());
            UTXORef utxo(addr, outpoint, input.value());

            _unconfirmedUtxoSet.erase(utxo);
            _confirmedUtxoSet.erase(utxo);
        }

        int32_t index = -1;
        for(const TXOutput & output : tx.outputs()) {
            index++;

            if(output.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            //we expect only one address in output.addresses() array
            QString addr = output.addresses()[0];
            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(txid, index);
            UTXORef utxo(addr, outpoint, output.value());

            if(output.spent_by()) continue;

            if(tx.confirmations() > 0) {
                _confirmedUtxoSet.insert(utxo);
            } else {
                _unconfirmedUtxoSet.insert(utxo);
            }
        }

        updateBalances(true);
    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateBalances(bool notify) {

        uint64_t confirmedBalance = 0;

        // Calculate new confirmed balance
        for(const UTXORef & utxo: _confirmedUtxoSet) {
            confirmedBalance += utxo.value();
        }

        // Calculate new unconfirmed balance
        uint64_t unconfirmedBalance = confirmedBalance;
        for(const UTXORef & utxo: _unconfirmedUtxoSet) {
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
