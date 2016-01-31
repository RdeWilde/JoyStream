/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef PAYMENT_CHANNEL_PAYOR_STATUS_HPP
#define PAYMENT_CHANNEL_PAYOR_STATUS_HPP

#include <extension/ChannelStatus.hpp>
#include <paymentchannel/PayorState.hpp>
#include <common/UnspentP2PKHOutput.hpp>

namespace joystream {
namespace extension {

    class PayorStatus {

    public:

        // Default constructor
        PayorStatus();

        // Construct from members
        PayorStatus(const std::vector<ChannelStatus> & channels,
               joystream::paymentchannel::PayorState state,
               const Coin::UnspentP2PKHOutput & utxo,
               quint64 changeValue,
               quint64 contractFee,
               const Coin::TransactionId & contractTxId,
               quint32 numberOfSignatures);

        // Getters and setters
        std::vector<ChannelStatus> channels() const;
        void setChannels(const std::vector<ChannelStatus> & channels);

        joystream::paymentchannel::PayorState state() const;
        void setState(joystream::paymentchannel::PayorState state);

        Coin::UnspentP2PKHOutput utxo() const;
        void setUtxo(const Coin::UnspentP2PKHOutput & utxo);

        quint64 changeValue() const;
        void setChangeValue(quint64 changeValue);

        quint64 contractFee() const;
        void setContractFee(quint64 contractFee);

        Coin::TransactionId contractTxId() const;
        void setContractTxId(const Coin::TransactionId & contractTxId);

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(quint32 numberOfSignatures);

    private:

        // Status of channels
        std::vector<ChannelStatus> _channels;

        // State of payor
        joystream::paymentchannel::PayorState _state;

        // Funding utxo
        Coin::UnspentP2PKHOutput _utxo;

        // Change amount sent back to payor,
        // this value, together with the _funds in all the slots
        // determines how much is paid in contract fee implicitly.
        quint64 _changeValue;

        // Contract fee
        quint64 _contractFee;

        // Transaction id of contract
        Coin::TransactionId _contractTxId;

        // Number of valid signatures collected
        quint32 _numberOfSignatures;
    };

}
}

#endif // PAYMENT_CHANNEL_PAYOR_STATUS_HPP

