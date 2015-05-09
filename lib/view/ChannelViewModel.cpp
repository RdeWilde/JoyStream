#include "ChannelViewModel.hpp"
#include "extension/BitCoin/BitCoin.hpp"

ChannelViewModel::ChannelViewModel(const Payor::Configuration & configuration)
    : _configuration(configuration) {
}

Payor::Channel::Configuration ChannelViewModel::configuration() const {
    return _configuration;
}

Payor::Channel::Status ChannelViewModel::status() const {
    return _status;
}

void ChannelViewModel::update(const Payor::Channel::Status & status) {

    quint32 index = status.index();

    Q_ASSERT(index == _configuration.index());

    if(_status.state() != status.state())
        emit stateChanged(index, status.state());

    if(_status.funds() != status.funds())
        emit fundsChanged(index, status.funds());

    if(_status.refundLockTime() != status.refundLockTime())
        emit refundLockTimeChanged(index, status.refundLockTime());

    bool balanceChanged = false;

    if(_status.price() != status.price()) {
        emit priceChanged(index, status.price());
        balanceChanged = true;
    }

    if(_status.numberOfPaymentsMade() != status.numberOfPaymentsMade()) {
        emit numberOfPaymentsMadeChanged(index, status.numberOfPaymentsMade());
        balanceChanged = true;
    }

    if(balanceChanged)
        emit balanceChanged(index, status.price()*status.numberOfPaymentsMade());

    // Save new status
    _status = status;
}
