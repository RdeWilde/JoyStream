/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#include <core/Payor.hpp>
#include <paymentchannel/paymentchannel.hpp>

Q_DECLARE_METATYPE(Coin::typesafeOutPoint)

namespace joystream {
namespace core {

void Payor::registerMetaTypes() {

    qRegisterMetaType<Coin::typesafeOutPoint>();
}

Payor::Payor(quint64 price,
             quint64 numberOfPaymentsMade,
             quint64 funds,
             quint64 settlementFee,
             quint16 refundLockTime,
             const Coin::typesafeOutPoint & anchor)
    : _price(price)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _funds(funds)
    , _settlementFee(settlementFee)
    , _refundLockTime(refundLockTime)
    , _anchor(anchor) {
}

Payor * Payor::create(const paymentchannel::Payor & payor) {
    return new Payor(payor.price(),
                     payor.numberOfPaymentsMade(),
                     payor.funds(),
                     payor.settlementFee(),
                     payor.refundLockTime(),
                     payor.anchor());
}

quint64 Payor::price() const noexcept {
    return _price;
}

quint64 Payor::numberOfPaymentsMade() const noexcept {
    return _numberOfPaymentsMade;
}

quint64 Payor::funds() const noexcept {
    return _funds;
}

quint64 Payor::settlementFee() const noexcept {
    return _settlementFee;
}

quint16 Payor::refundLockTime() const noexcept {
    return _refundLockTime;
}

Coin::typesafeOutPoint Payor::anchor() const noexcept {
    return _anchor;
}

void Payor::update(const paymentchannel::Payor & payor) {

    if(_price != payor.price()) {
        _price = payor.price();
        emit priceChanged(_price);
    }

    if(_numberOfPaymentsMade != payor.numberOfPaymentsMade()) {
        _numberOfPaymentsMade = payor.numberOfPaymentsMade();
        emit numberOfPaymentsMadeChanged(_numberOfPaymentsMade);
    }

    if(_funds != payor.funds()) {
        _funds = payor.funds();
        emit fundsChanged(_funds);
    }

    if(_settlementFee != payor.settlementFee()){
        _settlementFee = payor.settlementFee();
        emit settlementFeeChanged(_settlementFee);
    }

    if(_refundLockTime != payor.refundLockTime()) {
        _refundLockTime = payor.refundLockTime();
        emit refundLockTimeChanged(_refundLockTime);
    }

    if(_anchor != payor.anchor()) {
        _anchor = payor.anchor();
        emit anchorChanged(_anchor);
    }


}

}
}
