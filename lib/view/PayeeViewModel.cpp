#include "PayeeViewModel.hpp"

PayeeViewModel::PayeeViewModel(const Payee::Status & status)
    : _status(status) {
}

void PayeeViewModel::update(const Payee::Status & status) {

    if(_status.state() != status.state()) {
        _status.setState(status.state());
        emit stateChanged(status.state());
    }

    if(_status.numberOfPaymentsMade() != status.numberOfPaymentsMade()) {
        _status.setNumberOfPaymentsMade(status.numberOfPaymentsMade());
        emit numberOfPaymentsMadeChagned(status.numberOfPaymentsMade());
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

Payee::Status PayeeViewModel::status() const {
    return _status;
}

