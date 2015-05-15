#include "ChannelViewModel.hpp"
#include "extension/BitCoin/BitCoin.hpp"

ChannelViewModel::ChannelViewModel(QObject * parent,
                                   quint32 index,
                                   const Payor::Channel::Status & status)
    : QObject(parent)
    , _index(index)
    , _status(status) {
}

void ChannelViewModel::update(const Payor::Channel::Status & status) {

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

quint32 ChannelViewModel::index() const {
    return _index;
}

Payor::Channel::Status ChannelViewModel::status() const {
    return _status;
}
