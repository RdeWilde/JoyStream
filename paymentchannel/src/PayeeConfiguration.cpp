/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#include <paymentchannel/PayeeConfiguration.hpp>

namespace joystream {
namespace paymentchannel {

    PayeeConfiguration::PayeeConfiguration() {
    }

    PayeeConfiguration::PayeeConfiguration(PayeeState state,
                                        quint64 numberOfPaymentsMade,
                                        const Coin::Signature & lastValidPayorPaymentSignature,
                                        quint32 lockTime,
                                        quint64 price,
                                        quint32 maximumNumberOfSellers,
                                        const Coin::KeyPair & payeeContractKeys,
                                        const Coin::KeyPair & payeePaymentKeys,
                                        const Coin::typesafeOutPoint & contractOutPoint,
                                        const Coin::PublicKey & payorContractPk,
                                        const Coin::PublicKey & payorFinalPk,
                                        quint64 funds)
        : _state(state)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _lastValidPayorPaymentSignature(lastValidPayorPaymentSignature)
        , _lockTime(lockTime)
        , _price(price)
        , _maximumNumberOfSellers(maximumNumberOfSellers)
        , _payeeContractKeys(payeeContractKeys)
        , _payeePaymentKeys(payeePaymentKeys)
        , _contractOutPoint(contractOutPoint)
        , _payorContractPk(payorContractPk)
        , _payorFinalPk(payorFinalPk)
        , _funds(funds) {
    }

    PayeeState PayeeConfiguration::state() const {
        return _state;
    }

    void PayeeConfiguration::setState(PayeeState state) {
        _state = state;
    }

    quint64 PayeeConfiguration::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void PayeeConfiguration::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    Coin::Signature PayeeConfiguration::lastValidPayorPaymentSignature() const {
        return _lastValidPayorPaymentSignature;
    }

    void PayeeConfiguration::setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature) {
        _lastValidPayorPaymentSignature = lastValidPayorPaymentSignature;
    }

    quint32 PayeeConfiguration::lockTime() const {
        return _lockTime;
    }

    void PayeeConfiguration::setLockTime(quint32 lockTime) {
        _lockTime = lockTime;
    }

    quint64 PayeeConfiguration::price() const {
        return _price;
    }

    void PayeeConfiguration::setPrice(quint64 price) {
        _price = price;
    }

    quint32 PayeeConfiguration::maximumNumberOfSellers() const {
        return _maximumNumberOfSellers;
    }

    void PayeeConfiguration::setMaximumNumberOfSellers(quint32 maximumNumberOfSellers) {
        _maximumNumberOfSellers = maximumNumberOfSellers;
    }

    Coin::KeyPair PayeeConfiguration::payeeContractKeys() const {
        return _payeeContractKeys;
    }

    void PayeeConfiguration::setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys) {
        _payeeContractKeys = payeeContractKeys;
    }

    Coin::KeyPair PayeeConfiguration::payeePaymentKeys() const {
        return _payeePaymentKeys;
    }

    void PayeeConfiguration::setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys) {
        _payeePaymentKeys = payeePaymentKeys;
    }

    Coin::typesafeOutPoint PayeeConfiguration::contractOutPoint() const {
        return _contractOutPoint;
    }

    void PayeeConfiguration::setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint) {
        _contractOutPoint = contractOutPoint;
    }

    Coin::PublicKey PayeeConfiguration::payorContractPk() const {
        return _payorContractPk;
    }

    void PayeeConfiguration::setPayorContractPk(const Coin::PublicKey & payorContractPk) {
        _payorContractPk = payorContractPk;
    }

    Coin::PublicKey PayeeConfiguration::payorFinalPk() const {
        return _payorFinalPk;
    }

    void PayeeConfiguration::setPayorFinalPk(const Coin::PublicKey & payorFinalPk) {
        _payorFinalPk = payorFinalPk;
    }

    quint64 PayeeConfiguration::funds() const {
        return _funds;
    }

    void PayeeConfiguration::setFunds(const quint64 funds) {
        _funds = funds;
    }

}
}
