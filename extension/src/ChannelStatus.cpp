/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#include <extension/ChannelStatus.hpp>

namespace joystream {
namespace extension {

    ChannelStatus::ChannelStatus() {
    }

    ChannelStatus::ChannelStatus(quint32 index,
                                   joystream::paymentchannel::ChannelState state,
                                   quint64 price,
                                   quint64 numberOfPaymentsMade,
                                   quint64 funds,
                                   quint32 refundLockTime)
        : _index(index)
        , _state(state)
        , _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _refundLockTime(refundLockTime) {
    }

    quint32 ChannelStatus::refundLockTime() const {
        return _refundLockTime;
    }

    void ChannelStatus::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }

    quint64 ChannelStatus::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void ChannelStatus::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint64 ChannelStatus::funds() const {
        return _funds;
    }

    void ChannelStatus::setFunds(quint64 funds) {
        _funds = funds;
    }

    quint64 ChannelStatus::price() const {
        return _price;
    }

    void ChannelStatus::setPrice(quint64 price) {
        _price = price;
    }

    joystream::paymentchannel::ChannelState ChannelStatus::state() const {
        return _state;
    }

    void ChannelStatus::setState(joystream::paymentchannel::ChannelState state) {
        _state = state;
    }

    quint32 ChannelStatus::index() const {
        return _index;
    }

    void ChannelStatus::setIndex(quint32 index) {
        _index = index;
    }
}
}
