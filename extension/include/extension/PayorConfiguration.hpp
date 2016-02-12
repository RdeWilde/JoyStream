/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef JOYSTREAM_EXTENSION_PAYOR_CONFIGURATION_HPP
#define JOYSTREAM_EXTENSION_PAYOR_CONFIGURATION_HPP

#include <extension/ChannelConfiguration.hpp>
#include <paymentchannel/PayorState.hpp>
#include <common/Network.hpp>
#include <common/TransactionId.hpp>
#include <common/UnspentP2PKHOutput.hpp>

namespace joystream {
namespace extension {

    class PayorConfiguration {

    public:

        // Default constructor
        PayorConfiguration();

        // Constructor for a fresh payor.
        PayorConfiguration(Coin::Network network,
                          joystream::paymentchannel::PayorState state,
                          const std::vector<ChannelConfiguration> & channels,
                          const Coin::UnspentP2PKHOutput & utxo,
                          const Coin::KeyPair & changeOutputKeyPair,
                          quint64 changeValue,
                          quint64 contractFee,
                          const Coin::TransactionId & contractHash,
                          quint32 numberOfSignatures);

        // Getters and setters
        joystream::paymentchannel::PayorState state() const;
        void setState(joystream::paymentchannel::PayorState state);

        std::vector<ChannelConfiguration> channels() const;
        void setChannels(const std::vector<ChannelConfiguration> & channels);

        Coin::UnspentP2PKHOutput utxo() const;
        void setUtxo(const Coin::UnspentP2PKHOutput &utxo);

        /**
        OutPoint fundingOutPoint() const;
        void setFundingOutPoint(const OutPoint & fundingOutPoint);

        quint64 fundingValue() const;
        void setFundingValue(quint64 fundingValue);

        KeyPair fundingOutputKeyPair() const;
        void setFundingOutputKeyPair(const KeyPair & fundingOutputKeyPair);
        */

        Coin::KeyPair changeOutputKeyPair() const;
        void setChangeOutputKeyPair(const Coin::KeyPair & changeOutputKeyPair);

        quint64 changeValue() const;
        void setChangeValue(quint64 changeValue);

        quint64 contractFee() const;
        void setContractFee(quint64 contractFee);

        Coin::TransactionId contractHash() const;
        void setContractHash(const Coin::TransactionId & contractHash);

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(quint32 numberOfSignatures);


    private:

        // Why was this added, not sure it is needed?
        Coin::Network _network;

        // Payor state
        joystream::paymentchannel::PayorState _state;

        // Contract outputs
        std::vector<ChannelConfiguration> _channels;

        // Unspent output funding channel
        //OutPoint _fundingOutPoint;

        // Value of output funding channel (had to add due to bitcore requirements, remove later?)
        //quint64 _fundingValue;

        // Controls output funding channel
        //KeyPair _fundingOutputKeyPair;

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

        // Contract _contract;
        Coin::TransactionId _contractHash;

        // Number of valid refund signatures
        quint32 _numberOfSignatures;

    };

}
}

#endif // JOYSTREAM_EXTENSION_PAYOR_CONFIGURATION_HPP
