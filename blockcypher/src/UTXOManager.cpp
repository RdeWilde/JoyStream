/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <blockcypher/UTXOManager.hpp>

namespace BlockCypher {

    UTXOManager::UTXOManager(WebSocketClient * client, const std::set<Coin::P2PKHAddress> &addresses)
        : _wsclient(client),
          _balance(0),
          _balance_zero_conf(0)
    {
        for(const Coin::P2PKHAddress & address : addresses) {
            _addresses.insert(address.toBase58CheckEncoding());
        }

        // initialise utxomap from list of addresses
        InitialiseUtxo(_addresses, _txUtxoMap);

        // connect signals from websocket client to our private slots
        QObject::connect( _wsclient, &WebSocketClient::txArrived, [this](const TX & tx){
            onTransaction(tx);
        });
    }

    void UTXOManager::InitialiseUtxo(const std::set<QString> &addresses,
                                     txToUtxoMap_t & txUtxoMap) {

        /*
        1) build TXRef list from list of addresses (Manager::BLOCKCYPHER_rebuild_utxo).
           Basically requires a single call to Address Endpoint
           (http://dev.blockcypher.com/#address-endpoint),
           sending all addresses using batching (http://dev.blockcypher.com/#batching).

        2) build twoConf,oneConf,unconfirmed from TXRef list (see Manager::BLOCKCYPHER_rebuild_utxo)
        */
    }

    bool UTXOManager::addAddress(const Coin::P2PKHAddress & address) {
        QString addr = address.toBase58CheckEncoding();
        //return false if address already exists
        if(hasAddress(addr)) return false;

        //insert and add tx_confirmation event to websocket filters
        _addresses.insert(addr);
        _wsclient->addEvent(Event::makeTxConfirmation(2, addr));
        return true;
    }

    void UTXOManager::onTransaction(const TX & tx) {

        //confirmation status of tx
        uint32_t confirmations = tx.confirmations();
        Coin::TransactionId txid = Coin::TransactionId::fromRPCByteOrder(tx.hash().toStdString());

        //if we already have the tx, update the confirmations value
        if(_txUtxoMap.find(txid) != _txUtxoMap.end()) {
            _txUtxoMap[txid].confirmations(confirmations);
            return;
        }

        TransactionRelevantUtxo txEntry;

        txEntry.confirmations(confirmations);

        for(const TXInput & input : tx.inputs()) {
            if(input.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            //we expect only one address in output.addresses() array
            QString addr = input.addresses()[0];

            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(Coin::TransactionId::fromRPCByteOrder(input.prev_hash().toStdString()), input.index());
            UTXORef utxo(addr, outpoint, input.value());
            txEntry.destroys(utxo);
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
            txEntry.creates(utxo);
        }

        _txUtxoMap[txid] = txEntry;

        updateBalances();
    }

    void UTXOManager::onTxRef(const TXRef &txref) {

    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateBalances() {

    }
}
