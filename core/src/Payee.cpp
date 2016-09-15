/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2016
 */

#include <core/Payee.hpp>
#include <paymentchannel/paymentchannel.hpp>

Q_DECLARE_METATYPE(Coin::typesafeOutPoint)

namespace joystream {
namespace core {

void Payee::registerMetaTypes() {

    qRegisterMetaType<Coin::typesafeOutPoint>();
}

Payee::Payee(quint64 numberOfPaymentsMade,
             quint32 lockTime,
             quint64 price,
             quint64 funds,
             quint64 settlementFee,
             const Coin::typesafeOutPoint & anchor)
    : _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lockTime(lockTime)
    , _price(price)
    , _funds(funds)
    , _settlementFee(settlementFee)
    , _anchor(anchor) {
}

Payee * Payee::create(const paymentchannel::Payee & payee) {
    return new Payee(payee.numberOfPaymentsMade(),
                     payee.lockTime(),
                     payee.price(),
                     payee.funds(),
                     payee.settlementFee(),
                     payee.contractOutPoint());
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

Coin::typesafeOutPoint Payee::anchor() const noexcept {
    return _anchor;
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

    if(_anchor != payee.contractOutPoint()) {
        _anchor = payee.contractOutPoint();
        emit anchorChanged(_anchor);
    }
}

}
}
