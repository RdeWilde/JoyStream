/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYMENT_CHANNEL_PAYOR_HPP
#define PAYMENT_CHANNEL_PAYOR_HPP

#include <paymentchannel/Channel.hpp>
#include <common/KeyPair.hpp>
#include <common/Signature.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

namespace joystream {
namespace paymentchannel {

    class Contract;

    /**
     * Manages the payor side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/JoyStream/CBEP
     */
    class Payor {

    public:

        Payor(const std::vector<Channel> & channels,
              const Coin::UnspentP2PKHOutput & utxo,
              const Coin::KeyPair & changeOutputKeyPair,
              quint64 changeValue,
              quint64 contractFee,
              const Coin::Transaction & contractTx);

        // Creates and stores the contract transaction.
        // ===
        // Preserves fully valid contract, so that txid
        // can be anchor for all settelement/refund transactions.
        // Hence making this call will invalidate
        // any settlement/refund transactions.
        void setContractTransaction();

        // Creates and stores all payor refund signatures
        // ===
        // Not sure if this is actually needed really.
        void setAllPayorRefundSignatures();

        // Generates contract transaction
        Contract contract() const;

        // Getters and setters
        Channel & channel(int index);

        std::vector<Channel> getChannels() const;
        void setChannels(const std::vector<Channel> & channels);

        // The contract transaction
        Coin::Transaction getContractTx() const;

        // Transaction id of the contract
        Coin::TransactionId contractTxId() const;

    private:

        // Contract outputs
        std::vector<Channel> _channels;

        // Funding
        Coin::UnspentP2PKHOutput _utxo;

        // Controls change output in contract
        Coin::KeyPair _changeOutputKeyPair;

        // Change amount sent back to payor,
        // this value, together with the _funds in all the slots
        // determines how much is paid in contract fee implicitly.
        quint64 _changeValue;

        // Contract fee
        quint64 _contractFee;

        // Contract transaction
        // Everything except the input signatures can be rederived,
        // however the input signatures cannot due to non-determinism in
        // ECDSA signing. Hence to preserve contract txid, they have to be stored,
        // so we just store full transaction.
        Coin::Transaction _contractTx;
    };

}
}

#endif // PAYMENT_CHANNEL_PAYOR_HPP
