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
    {}

    UTXOManager* UTXOManager::createManager(WebSocketClient * wsclient,
                                     const std::set<Coin::P2PKHAddress> &p2pkhaddresses,
                                     Client * restClient)
    {

        UTXOManager* manager = new UTXOManager(wsclient);

        if(!manager) throw std::runtime_error("unable to create a UTXO Manager");

        std::set<QString> addresses;
        for(const Coin::P2PKHAddress & address : p2pkhaddresses) {
            addresses.insert(address.toBase58CheckEncoding());
        }

        // If REST api client is provided we can initialise the utxo state
        if(restClient) {
            if(!manager->refreshUtxoState(restClient, addresses)) {
                delete manager;
                return nullptr;
            }
        }

        // add initial addresses after successful initialization so
        // we don't create multiple events in the websocket for the
        // same address
        for(const QString & address : addresses) {
            manager->addAddress(address);
        }

        // connect websocket signals only after successful initialization
        manager->listenForEvents();

        return manager;
    }

    void UTXOManager::listenForEvents() {
        // connect signals from websocket client to our private slots
        QObject::connect(_wsClient, &WebSocketClient::txArrived, [this](const TX & tx){
            TxResult r = processTx(tx);
            updateUtxoSets(r);
            updateBalances(true);
        });
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

    std::vector<TXRef> UTXOManager::fetchTxRefs(Client * restClient, const std::set<QString> & addresses) {
        return fetchTxRefs(restClient, batchAddresses(addresses));
    }

    bool UTXOManager::refreshUtxoState(Client* restClient) {
        return refreshUtxoState(restClient, _addresses);
    }

    bool UTXOManager::refreshUtxoState(Client* restClient, const std::set<QString> & addresses) {
        //use a mutex or bool flag to stop updates from wsclient while we reset the state?
        std::vector<TXRef> txrefs;

        try {
            txrefs = fetchTxRefs(restClient, addresses);
        } catch(std::exception &e) {
            return false;
        }

        std::vector<TxResult> results = processTxRefs(txrefs);

        _unconfirmedUtxoSet.clear();
        _confirmedUtxoSet.clear();

        for(auto & r : results) updateUtxoSets(r);

        updateBalances(true);

        return true;
    }

    std::set<UTXO> UTXOManager::getUtxoSet(uint64_t minValue, uint32_t minConfirmations, uint32_t currentBlockHeight) {
        std::set<UTXO> selectedUtxos;
        uint32_t totalValue = 0;
        uint32_t confirmations;

        // Always try to pick confirmed utxos
        for(const UTXO &utxo : _confirmedUtxoSet) {
            // is utxo loacked ?
            if(_lockedUtxoSet.find(utxo) != _lockedUtxoSet.end()) continue;

            confirmations = currentBlockHeight - utxo.height() + 1;

            // Does it meet minimal confirmations requirement
            if(confirmations < minConfirmations) continue;

            selectedUtxos.insert(utxo);
            totalValue += utxo.value();

            if(totalValue >= minValue) {
                break;
            }
        }

        // select unconfirmed utxo if minimal confirmations requirement == 0
        // and we still haven't reached minimum required value
        if(minConfirmations == 0 && totalValue < minValue) {
            for(const UTXO &utxo : _unconfirmedUtxoSet) {
                if(_lockedUtxoSet.find(utxo) != _lockedUtxoSet.end()) continue;

                selectedUtxos.insert(utxo);
                totalValue += utxo.value();

                if(totalValue >= minValue) {
                    break;
                }
            }
        }

        if(totalValue < minValue) {
            // not enough utxo
            return std::set<UTXO>();
        }

        // Lock and return the selected utxos
        lockUtxoSet(selectedUtxos);
        return selectedUtxos;
    }

    void UTXOManager::releaseUtxoSet(std::set<UTXO> utxos) {
        for(auto & utxo : utxos) _lockedUtxoSet.erase(utxo);
    }

    void UTXOManager::lockUtxoSet(std::set<UTXO> utxos) {
        for(auto & utxo : utxos) _lockedUtxoSet.insert(utxo);
    }

    std::vector<TxResult> UTXOManager::processTxRefs(const std::vector<TXRef> &txrefs) {
        std::vector<TxResult> results;

        for(const TXRef &t : txrefs) {
            if(t._tx_output_n >= 0 && t._spent == false) {
                Coin::typesafeOutPoint outpoint(t._tx_hash, t._tx_output_n);
                UTXO utxo(QString::fromStdString(t._addressString), outpoint, t._value, t._block_height);
                TxResult result;
                result.confirmations(t._confirmations);
                result.creates(utxo);
                results.push_back(result);
            }
        }

        return results;
    }

    bool UTXOManager::addAddress(const Coin::P2PKHAddress & address) {
        return addAddress(address.toBase58CheckEncoding());
    }

    bool UTXOManager::addAddress(const QString & addr) {
        //return false if address already exists
        if(hasAddress(addr)) return false;

        //insert address and add tx_confirmation event to websocket filters
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

            if(output.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            //we expect only one address in output.addresses() array
            QString addr = output.addresses()[0];
            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(txid, index);
            UTXO utxo(addr, outpoint, output.value(), tx.block_height());

            if(output.spent_by()) {
                result.destroys(utxo);
            } else {
                result.creates(utxo);
            }
        }

        return result;
    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateUtxoSets(const TxResult & r) {

        for(const UTXO &utxo : r.creates()) {
            if(utxo.confirmed()) {
                // erasing and inserting to update the block height of the utxo
                // incase of reorg
                _confirmedUtxoSet.erase(utxo);
                _confirmedUtxoSet.insert(utxo);

                _unconfirmedUtxoSet.erase(utxo);
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
