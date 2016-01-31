/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#include <paymentchannel/ChannelConfiguration.hpp>

namespace joystream {
namespace paymentchannel {

    ChannelConfiguration::ChannelConfiguration() {
    }

    ChannelConfiguration::ChannelConfiguration(quint32 index,
                                            ChannelState state,
                                            quint64 price,
                                            quint64 numberOfPaymentsMade,
                                            quint64 funds,
                                            const Coin::KeyPair & payorContractKeyPair,
                                            const Coin::KeyPair & payorFinalKeyPair,
                                            const Coin::PublicKey & payeeContractPk,
                                            const Coin::PublicKey & payeeFinalPk,
                                            const Coin::Signature & payorRefundSignature,
                                            const Coin::Signature & payeeRefundSignature,
                                            quint64 refundFee,
                                            quint64 paymentFee,
                                            quint32 refundLockTime)
                : _index(index)
                , _state(state)
                , _price(price)
                , _numberOfPaymentsMade(numberOfPaymentsMade)
                , _funds(funds)
                , _payorContractKeyPair(payorContractKeyPair)
                , _payorFinalKeyPair(payorFinalKeyPair)
                , _payeeContractPk(payeeContractPk)
                , _payeeFinalPk(payeeFinalPk)
                , _payorRefundSignature(payorRefundSignature)
                , _payeeRefundSignature(payeeRefundSignature)
                , _refundFee(refundFee)
                , _paymentFee(paymentFee)
                , _refundLockTime(refundLockTime) {
    }

    quint32 ChannelConfiguration::index() const {
        return _index;
    }

    void ChannelConfiguration::setIndex(quint32 index) {
        _index = index;
    }

    ChannelState ChannelConfiguration::state() const {
        return _state;
    }

    void ChannelConfiguration::setState(ChannelState state) {
        _state =  state;
    }

    quint64 ChannelConfiguration::price() const {
        return _price;
    }

    void ChannelConfiguration::setPrice(quint64 price) {
        _price = price;
    }

    quint64 ChannelConfiguration::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void ChannelConfiguration::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint64 ChannelConfiguration::funds() const {
        return _funds;
    }

    void ChannelConfiguration::setFunds(quint64 funds) {
        _funds = funds;
    }

    Coin::KeyPair ChannelConfiguration::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void ChannelConfiguration::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::KeyPair ChannelConfiguration::payorFinalKeyPair() const {
        return _payorFinalKeyPair;
    }

    void ChannelConfiguration::setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair) {
        _payorFinalKeyPair = payorFinalKeyPair;
    }

    Coin::PublicKey ChannelConfiguration::payeeContractPk() const {
        return _payeeContractPk;
    }

    void ChannelConfiguration::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PublicKey ChannelConfiguration::payeeFinalPk() const {
        return _payeeFinalPk;
    }

    void ChannelConfiguration::setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk) {
        _payeeFinalPk = payeeFinalPk;
    }

    Coin::Signature ChannelConfiguration::payorRefundSignature() const {
        return _payorRefundSignature;
    }

    void ChannelConfiguration::setPayorRefundSignature(const Coin::Signature & payorRefundSignature) {
        _payorRefundSignature = payorRefundSignature;
    }

    Coin::Signature ChannelConfiguration::payeeRefundSignature() const {
        return _payeeRefundSignature;
    }

    void ChannelConfiguration::setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature) {
        _payeeRefundSignature = payeeRefundSignature;
    }

    quint64 ChannelConfiguration::refundFee() const {
        return _refundFee;
    }

    void ChannelConfiguration::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }

    quint64 ChannelConfiguration::paymentFee() const {
        return _paymentFee;
    }

    void ChannelConfiguration::setPaymentFee(quint64 paymentFee) {
        _paymentFee = paymentFee;
    }

    quint32 ChannelConfiguration::refundLockTime() const {
        return _refundLockTime;
    }

    void ChannelConfiguration::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }
}
}
