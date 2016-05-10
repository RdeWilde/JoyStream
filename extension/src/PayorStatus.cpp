/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef EXTENSION_PAYOR_STATUS_HPP
#define EXTENSION_PAYOR_STATUS_HPP

#include <extension/PayorStatus.hpp>

namespace joystream {
namespace extension {

    PayorStatus::PayorStatus() {
    }

    PayorStatus::PayorStatus(const std::vector<ChannelStatus> & channels,
                          joystream::paymentchannel::PayorState state,
                          const Coin::UnspentP2PKHOutput & utxo,
                          quint64 changeValue,
                          quint64 contractFee,
                          const Coin::TransactionId & contractTxId,
                          quint32 numberOfSignatures)
        : _channels(channels)
        , _state(state)
        , _utxo(utxo)
        , _changeValue(changeValue)
        , _contractFee(contractFee)
        , _contractTxId(contractTxId)
        , _numberOfSignatures(numberOfSignatures) {
    }

    std::vector<ChannelStatus> PayorStatus::channels() const {
        return _channels;
    }

    void PayorStatus::setChannels(const std::vector<ChannelStatus> & channels) {
        _channels = channels;
    }

    joystream::paymentchannel::PayorState PayorStatus::state() const {
        return _state;
    }

    void PayorStatus::setState(joystream::paymentchannel::PayorState state) {
        _state = state;
    }

    Coin::UnspentP2PKHOutput PayorStatus::utxo() const {
        return _utxo;
    }

    void PayorStatus::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
        _utxo = utxo;
    }

    quint64 PayorStatus::changeValue() const {
        return _changeValue;
    }

    void PayorStatus::setChangeValue(quint64 changeValue) {
        _changeValue = changeValue;
    }

    quint64 PayorStatus::contractFee() const {
        return _contractFee;
    }

    void PayorStatus::setContractFee(quint64 contractFee) {
        _contractFee = contractFee;
    }

    Coin::TransactionId PayorStatus::contractTxId() const {
        return _contractTxId;
    }

    void PayorStatus::setContractTxId(const Coin::TransactionId & contractTxId) {
        _contractTxId = contractTxId;
    }

    quint32 PayorStatus::numberOfSignatures() const {
        return _numberOfSignatures;
    }

    void PayorStatus::setNumberOfSignatures(quint32 numberOfSignatures) {
        _numberOfSignatures = numberOfSignatures;
    }

}
}

#endif // EXTENSION_PAYOR_STATUS_HPP

