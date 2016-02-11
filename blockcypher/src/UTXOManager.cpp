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
        InitialiseUtxo(_addresses, _confirmed_utxo_set, _unconfirmed_utxo_set);

        // connect signals from websocket client to our private slots
        QObject::connect( _wsclient, &WebSocketClient::txArrived, [this](const TX & tx){
            processTx(tx);
        });
    }

    void UTXOManager::InitialiseUtxo(const std::set<QString> &addresses,
                                     std::set<UTXORef> &confirmedSet, std::set<UTXORef> &unconfirmedSet) {

        /*
        1) build TXRef list from list of addresses (Manager::BLOCKCYPHER_rebuild_utxo).
           Basically requires a single call to Address Endpoint
           (http://dev.blockcypher.com/#address-endpoint),
           sending all addresses using batching (http://dev.blockcypher.com/#batching).

        2) build confirmedSet and unconfirmedSet from TXRef list (see Manager::BLOCKCYPHER_rebuild_utxo)
        */

        std::vector<BlockCypher::TXRef> txrefs;

        for(const TXRef &t : txrefs) {
            if(t._tx_output_n >= 0 && t._spent == false) {

                Coin::typesafeOutPoint outpoint(t._tx_hash, t._tx_output_n);
                UTXORef utxo(QString::fromStdString(t._addressString), outpoint, t._value);

                if(t._confirmations > 0) {
                    confirmedSet.insert(utxo);
                } else {
                    unconfirmedSet.insert(utxo);
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
        _wsclient->addEvent(Event::makeTxConfirmation(2, addr));
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

            _unconfirmed_utxo_set.erase(utxo);
            _confirmed_utxo_set.erase(utxo);
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

            if(output.spent_by()){
                _unconfirmed_utxo_set.erase(utxo);
                _confirmed_utxo_set.erase(utxo);
                return;
            }

            if(tx.confirmations() > 0) {
                _confirmed_utxo_set.insert(utxo);
            } else {
                _unconfirmed_utxo_set.insert(utxo);
            }
        }

        updateBalances();
    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateBalances() {

    }
}
