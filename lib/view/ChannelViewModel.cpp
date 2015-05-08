#include "ChannelViewModel.hpp"
#include "PayorViewModel.hpp"

#include "extension/BitCoin/BitCoin.hpp"

ChannelViewModel::ChannelViewModel(PayorViewModel * model)//, libtorrent::tcp::endpoint & endPoint)
    : _hostItem(new QStandardItem())
    , _indexItem(new QStandardItem())
    , _stateItem(new QStandardItem())
    , _fundsItem(new QStandardItem())
    , _refundLockTimeItem(new QStandardItem())
    , _priceItem(new QStandardItem())
    , _numberOfPaymentsMadeItem(new QStandardItem())
    , _balanceItem(new QStandardItem())
    , _uploadSpeedItem(new QStandardItem()) {

    // Add as row to model
    QList<QStandardItem *> row;

    row.append(_hostItem);
    row.append(_indexItem);
    row.append(_stateItem);
    row.append(_fundsItem);
    row.append(_refundLockTimeItem);
    row.append(_priceItem);
    row.append(_numberOfPaymentsMadeItem);
    row.append(_balanceItem);
    row.append(_uploadSpeedItem);

    model->channelTableViewModel()->appendRow(row);
}

void ChannelViewModel::update(const Payor::Channel::Status & status) {

    updateIndexItem(status.index());
    updateStateItem(status.state());
    updateFundsItem(status.funds());
    updateRefundLockTime(status.refundLockTime());

    // Only update price if channel is assigned to someone
    if(status.state() != Payor::Channel::State::unassigned) {

        updatePriceItem(status.price());
        updateNumberOfPaymentsMadeItem(status.numberOfPaymentsMade());
        updateBalanceItem(status.price() * status.numberOfPaymentsMade());

    } else {
        Q_ASSERT(status.numberOfPaymentsMade() == 0);
    }
}


void ChannelViewModel::updateHostItem(libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(endPointString));
}

void ChannelViewModel::updateIndexItem(quint32 index) {
    _indexItem->setText(QString::number(index));
}

void ChannelViewModel::updateStateItem(Payor::Channel::State state) {

    // Get text representation of state
    QString channelState;

    switch(state) {

        case Payor::Channel::State::unassigned:
            channelState = "Unassigned";
            break;

        case Payor::Channel::State::assigned:
            channelState = "Assigned";
            break;

        case Payor::Channel::State::refund_signed:
            channelState = "Refund signed";
            break;
    }

    // Update model view
    _stateItem->setText(channelState);
}

void ChannelViewModel::updateFundsItem(quint64 funds) {
    _fundsItem->setText(QString::number(funds));
}

void ChannelViewModel::updateRefundLockTime(quint32 refundLockTime) {
    _refundLockTimeItem->setText(QString::number(refundLockTime));
}

void ChannelViewModel::updatePriceItem(quint64 price) {
    _priceItem->setText(QString::number(price));
}

void ChannelViewModel::updateNumberOfPaymentsMadeItem(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMadeItem->setText(QString::number(numberOfPaymentsMade));
}

void ChannelViewModel::updateBalanceItem(quint64 balance) {
    _balanceItem->setText(QString::number(balance));
}

void ChannelViewModel::updateUploadSpeedItem(quint64 bytesPrSecond) {
    _uploadSpeedItem->setText(QString::number(bytesPrSecond));
}
