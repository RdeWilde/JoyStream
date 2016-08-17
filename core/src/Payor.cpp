/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#include <core/Payor.hpp>
#include <paymentchannel/paymentchannel.hpp>

namespace joystream {
namespace core {

Payor::Payor(const paymentchannel::Payor & payor)
    : _price(payor.price())
    , _numberOfPaymentsMade(payor.numberOfPaymentsMade())
    , _funds(payor.funds())
    , _settlementFee(payor.settlementFee())
    , _refundLockTime(payor.refundLockTime())
    , _anchor(payor.anchor()) {
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

quint32 Payor::refundLockTime() const noexcept {
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
