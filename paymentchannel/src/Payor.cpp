/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Contract.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>

namespace joystream {
namespace paymentchannel {

    Payor::Payor(const std::vector<Channel> & channels,
                 const Coin::UnspentP2PKHOutput & utxo,
                 const Coin::KeyPair & changeOutputKeyPair,
                 quint64 changeValue,
                 quint64 contractFee,
                 const Coin::Transaction & contractTx)
        : _channels(channels)
        , _utxo(utxo)
        , _changeOutputKeyPair(changeOutputKeyPair)
        , _changeValue(changeValue)
        , _contractFee(contractFee)
        , _contractTx(contractTx) {
    }

    void Payor::anchor() {

        // Generate the contract transaction, and store
        Contract c = contract();

        // Store transaction
        _contractTx = c.transaction();

        Coin::TransactionId txId = Coin::TransactionId::fromTx(_contractTx);

        // Set contract transaction id in each channel
        for(std::vector<Channel>::iterator i = _channels.begin(), end(_channels.end()); i != end;i++)
            i->setContractTxId(txId);
    }

    void Payor::setAllPayorRefundSignatures() {

        for(std::vector<Channel>::iterator i = _channels.begin(), end(_channels.end()); i != end;i++) {

            // Generate refund signature for payor
            Coin::Signature sig = i->generatePayorRefundSignature();

            // Save signature in channel
            i->setPayorRefundSignature(sig);
        }
    }

    Contract Payor::contract() const {

        // Collect channel commitments
        std::vector<Commitment> commitments;

        for(std::vector<Channel>::const_iterator i =  _channels.cbegin(), end = _channels.cend(); i != end; i++)
            commitments.push_back((*i).commitment());

        // Build contract
        return Contract(_utxo, commitments, Coin::Payment(_changeValue, _changeOutputKeyPair.pk().toPubKeyHash()));
    }

    Channel & Payor::channel(int index) {

        if(index < 0 && index > _channels.size())
            throw std::runtime_error("Invalid channel index");

        return _channels[index];
    }

    int Payor::numberOfChannel() const {
        return _channels.size();
    }

    std::vector<Channel> Payor::getChannels() const {
        return _channels;
    }

    void Payor::setChannels(const std::vector<Channel> & channels) {
        _channels = channels;
    }

    Coin::Transaction Payor::getContractTx() const {
        return _contractTx;
    }
}
}
