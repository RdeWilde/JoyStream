/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2016
 */

#include <core/Payee.hpp>
#include <paymentchannel/paymentchannel.hpp>

namespace joystream {
namespace core {

Payee::Payee(const paymentchannel::Payee & payee)
    : _numberOfPaymentsMade(payee.numberOfPaymentsMade())
    , _lockTime(payee.lockTime())
    , _price(payee.price())
    , _funds(payee.funds())
    , _settlementFee(payee.settlementFee()) {
}

quint64 Payee::numberOfPaymentsMade() const noexcept {
    return _numberOfPaymentsMade;
}

quint32 Payee::lockTime() const noexcept {
    return _lockTime;
}

quint64 Payee::price() const noexcept {
    return _price;
}

quint64 Payee::funds() const noexcept {
    return _funds;
}

quint64 Payee::settlementFee() const noexcept {
    return _settlementFee;
}

void Payee::update(const paymentchannel::Payee & payee) {

    if(_numberOfPaymentsMade != payee.numberOfPaymentsMade()) {
        _numberOfPaymentsMade = payee.numberOfPaymentsMade();
        emit numberOfPaymentsMadeChanged(_numberOfPaymentsMade);
    }

    if(_lockTime != payee.lockTime()) {
        _lockTime = payee.lockTime();
        emit lockTimeChanged(_lockTime);
    }

    if(_price != payee.price()) {
        _price = payee.price();
        emit priceChanged(_price);
    }

    if(_funds != payee.funds()) {
        _funds = payee.funds();
        emit fundsChanged(_funds);
    }

    if(_settlementFee != payee.settlementFee()) {
        _settlementFee = payee.settlementFee();
        emit settlementFeeChanged(_settlementFee);
    }
}

}
}
