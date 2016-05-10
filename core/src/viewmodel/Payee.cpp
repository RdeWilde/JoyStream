/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/viewmodel/Payee.hpp>

namespace joystream {
namespace core {
namespace viewmodel {

    Payee::Payee(QObject * parent, const joystream::extension::PayeeStatus & status)
        : QObject(parent)
        , _status(status) {
    }

    void Payee::update(const joystream::extension::PayeeStatus & status) {

        quint64 oldBalance = _status.price()*_status.numberOfPaymentsMade();
        quint64 newBalance = status.price()*status.numberOfPaymentsMade();

        if(oldBalance != newBalance) {
            emit balanceChanged(newBalance);
        }

        if(_status.state() != status.state()) {
            _status.setState(status.state());
            emit stateChanged(status.state());
        }

        if(_status.contractOutPoint() != status.contractOutPoint()) {
            _status.setContractOutPoint(status.contractOutPoint());
            emit contractOutPointChanged(status.contractOutPoint());
        }

        if(_status.numberOfPaymentsMade() != status.numberOfPaymentsMade()) {
            _status.setNumberOfPaymentsMade(status.numberOfPaymentsMade());
            emit numberOfPaymentsMadeChanged(status.numberOfPaymentsMade());
        }

        if(_status.lockTime() != status.lockTime()) {
            _status.setLockTime(status.lockTime());
            emit lockTimeChanged(status.lockTime());
        }

        if(_status.price() != status.price()) {
            _status.setPrice(status.price());
            emit priceChanged(status.price());
        }

        if(_status.funds() != status.funds()) {
            _status.setFunds(status.funds());
            emit fundsChanged(status.funds());
        }
    }

    joystream::extension::PayeeStatus Payee::status() const {
        return _status;
    }

}
}
}
