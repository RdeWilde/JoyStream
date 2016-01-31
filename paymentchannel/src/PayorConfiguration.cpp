/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 *
 *
 */

#include <paymentchannel/PayorConfiguration.hpp>

namespace joystream {
namespace paymentchannel {

    PayorConfiguration::PayorConfiguration() {

    }

    PayorConfiguration::PayorConfiguration(Coin::Network network,
                                        PayorState state,
                                        const std::vector<ChannelConfiguration> & channels,
                                        const Coin::UnspentP2PKHOutput & utxo,
                                        //const OutPoint & fundingOutPoint,
                                        //quint64 fundingValue,
                                        //const KeyPair & fundingOutputKeyPair,
                                        const Coin::KeyPair & changeOutputKeyPair,
                                        quint64 changeValue,
                                        quint64 contractFee,
                                        const Coin::TransactionId & contractHash,
                                        quint32 numberOfSignatures)
        : _network(network)
        , _state(state)
        , _channels(channels)
        //, _fundingOutPoint(fundingOutPoint)
        //, _fundingValue(fundingValue)
        //, _fundingOutputKeyPair(fundingOutputKeyPair)
        , _utxo(utxo)
        , _changeOutputKeyPair(changeOutputKeyPair)
        , _changeValue(changeValue)
        , _contractFee(contractFee)
        , _contractHash(contractHash)
        , _numberOfSignatures(numberOfSignatures) {
    }

    PayorState PayorConfiguration::state() const {
        return _state;
    }

    void PayorConfiguration::setState(PayorState state) {
        _state = state;
    }

    std::vector<ChannelConfiguration> PayorConfiguration::channels() const {
        return _channels;
    }

    void PayorConfiguration::setChannels(const std::vector<ChannelConfiguration> & channels) {
        _channels = channels;
    }

    Coin::UnspentP2PKHOutput PayorConfiguration::utxo() const {
        return _utxo;
    }

    void PayorConfiguration::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
        _utxo = utxo;
    }

    /**
    OutPoint Payor::fundingOutPoint() const {
        return _fundingOutPoint;
    }

    void Payor::setFundingOutPoint(const OutPoint & fundingOutPoint) {
        _fundingOutPoint = fundingOutPoint;
    }

    quint64 Payor::fundingValue() const {
        return _fundingValue;
    }

    void Payor::setFundingValue(quint64 fundingValue) {
        _fundingValue = fundingValue;
    }

    KeyPair Payor::fundingOutputKeyPair() const {
        return _fundingOutputKeyPair;
    }

    void Payor::setFundingOutputKeyPair(const KeyPair & fundingOutputKeyPair) {
        _fundingOutputKeyPair = fundingOutputKeyPair;
    }
    */

    Coin::KeyPair PayorConfiguration::changeOutputKeyPair() const {
        return _changeOutputKeyPair;
    }

    void PayorConfiguration::setChangeOutputKeyPair(const Coin::KeyPair & changeOutputKeyPair) {
        _changeOutputKeyPair = changeOutputKeyPair;
    }

    quint64 PayorConfiguration::changeValue() const {
        return _changeValue;
    }

    void PayorConfiguration::setChangeValue(quint64 changeValue) {
        _changeValue = changeValue;
    }

    quint64 PayorConfiguration::contractFee() const {
        return _contractFee;
    }

    void PayorConfiguration::setContractFee(quint64 contractFee) {
        _contractFee = contractFee;
    }

    Coin::TransactionId PayorConfiguration::contractHash() const {
        return _contractHash;
    }

    void PayorConfiguration::setContractHash(const Coin::TransactionId & contractHash) {
        _contractHash = contractHash;
    }

    quint32 PayorConfiguration::numberOfSignatures() const {
        return _numberOfSignatures;
    }

    void PayorConfiguration::setNumberOfSignatures(quint32 numberOfSignatures) {
        _numberOfSignatures = numberOfSignatures;
    }

}
}
