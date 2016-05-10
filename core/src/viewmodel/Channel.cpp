/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/viewmodel/Channel.hpp>
#include <common/Bitcoin.hpp>

namespace joystream {
namespace core {
namespace viewmodel {

    Channel::Channel(QObject * parent,
                     quint32 index,
                     const joystream::extension::ChannelStatus & status)
        : QObject(parent)
        , _index(index)
        , _status(status) {
    }

    void Channel::update(const joystream::extension::ChannelStatus & status) {

        if(_status.state() != status.state()) {
            _status.setState(status.state());
            emit stateChanged(status.state());
        }

        if(_status.funds() != status.funds()) {
            _status.setFunds(status.funds());
            emit fundsChanged(status.funds());
        }

        if(_status.refundLockTime() != status.refundLockTime()) {
            _status.setRefundLockTime(status.refundLockTime());
            emit refundLockTimeChanged(status.refundLockTime());
        }

        bool updateBalance = false;

        if(_status.price() != status.price()) {
            _status.setPrice(status.price());
            emit priceChanged(status.price());
            updateBalance = true;
        }

        if(_status.numberOfPaymentsMade() != status.numberOfPaymentsMade()) {
            _status.setNumberOfPaymentsMade(status.numberOfPaymentsMade());
            emit numberOfPaymentsMadeChanged(status.numberOfPaymentsMade());
            updateBalance = true;
        }

        if(updateBalance)
            emit balanceChanged(status.price()*status.numberOfPaymentsMade());
    }

    quint32 Channel::index() const {
        return _index;
    }

    joystream::extension::ChannelStatus Channel::status() const {
        return _status;
    }

}
}
}
