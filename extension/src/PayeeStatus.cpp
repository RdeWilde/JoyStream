/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#include <extension/PayeeStatus.hpp>

namespace joystream {
namespace extension {

    PayeeStatus::PayeeStatus() {
    }

    PayeeStatus::PayeeStatus(joystream::paymentchannel::PayeeState state, quint64 numberOfPaymentsMade, quint32 lockTime, quint64 price, const Coin::typesafeOutPoint & contractOutPoint, quint64 funds)
        : _state(state)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _lockTime(lockTime)
        , _price(price)
        , _contractOutPoint(contractOutPoint)
        , _funds(funds) {
    }

    joystream::paymentchannel::PayeeState PayeeStatus::state() const {
        return _state;
    }

    void PayeeStatus::setState(const joystream::paymentchannel::PayeeState state) {
        _state = state;
    }

    quint64 PayeeStatus::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void PayeeStatus::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint32 PayeeStatus::lockTime() const {
        return _lockTime;
    }

    void PayeeStatus::setLockTime(quint32 lockTime) {
        _lockTime = lockTime;
    }

    quint64 PayeeStatus::price() const {
        return _price;
    }

    void PayeeStatus::setPrice(quint64 price) {
        _price = price;
    }

    Coin::typesafeOutPoint PayeeStatus::contractOutPoint() const {
        return _contractOutPoint;
    }

    void PayeeStatus::setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint) {
        _contractOutPoint = contractOutPoint;
    }

    quint64 PayeeStatus::funds() const {
        return _funds;
    }

    void PayeeStatus::setFunds(quint64 funds) {
        _funds = funds;
    }

}
}
