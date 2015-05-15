#include "ChannelView.hpp"
#include "controller/ChannelViewModel.hpp"

#include <QStandardItem>
#include <QStandardItemModel>

ChannelView::ChannelView(QObject * parent,
                         const ChannelViewModel * channelViewModel,
                         QStandardItemModel * model)
    : QObject(parent)
    , _index(channelViewModel->index())
    , _stateItem(new QStandardItem(stateToString(channelViewModel->status().state())))
    , _fundsItem(new QStandardItem(fundsToString(channelViewModel->status().funds())))
    , _refundLockTimeItem(new QStandardItem(refundLockTimeToString(channelViewModel->status().refundLockTime())))
    , _priceItem(new QStandardItem(priceToString(channelViewModel->status().price())))
    , _numberOfPaymentMadeItem(new QStandardItem(numberOfPaymentsMadeToString(channelViewModel->status().numberOfPaymentsMade())))
    , _balanceItem(new QStandardItem(balanceToString(channelViewModel->status().price() * channelViewModel->status().numberOfPaymentsMade()))) {

    // Add row to model
    QList<QStandardItem *> items;

    items << _stateItem
          << _fundsItem
          << _refundLockTimeItem
          << _priceItem
          << _numberOfPaymentMadeItem
          << _balanceItem;

    model->appendRow(items);

    // Connect view model signals to slots
    QObject::connect(channelViewModel,
                     SIGNAL(stateChanged(Payor::Channel::State)),
                     this,
                     SLOT(updateState(Payor::Channel::State)));

    QObject::connect(channelViewModel,
                     SIGNAL(fundsChanged(quint64)),
                     this,
                     SLOT(updateFunds(quint64)));

    QObject::connect(channelViewModel,
                     SIGNAL(refundLockTimeChanged(quint32)),
                     this,
                     SLOT(updateRefundLockTime(quint32)));

    QObject::connect(channelViewModel,
                     SIGNAL(priceChanged(quint64)),
                     this,
                     SLOT(updatePrice(quint64)));

    QObject::connect(channelViewModel,
                     SIGNAL(numberOfPaymentsMadeChanged(quint64)),
                     this,
                     SLOT(updateNumberOfPaymentsMade(quint64)));

    QObject::connect(channelViewModel,
                     SIGNAL(balanceChanged(quint64)),
                     this,
                     SLOT(updateBalance(quint64)));

}

QString ChannelView::stateToString(Payor::Channel::State state) {

    QString text;

    switch(state) {
        case Payor::Channel::State::unassigned:
            text = "Unassigned";
            break;
        case Payor::Channel::State::assigned:
            text = "Assigned";
            break;
        case Payor::Channel::State::refund_signed:
            text = "Refund Signed";
            break;
    }

    return text;
}

QString ChannelView::fundsToString(quint64 funds) {
    return QString::number(funds);
}

QString ChannelView::refundLockTimeToString(quint32 refundLockTime) {
    return QString::number(refundLockTime);
}

QString ChannelView::priceToString(quint64 price) {
    return QString::number(price);
}

QString ChannelView::numberOfPaymentsMadeToString(quint64 numberOfPaymentsMade) {
    return QString::number(numberOfPaymentsMade);
}

QString ChannelView::balanceToString(quint64 balance) {
    return QString::number(balance);
}

void ChannelView::updateState(Payor::Channel::State state) {
    _stateItem->setText(stateToString(state));
}

void ChannelView::updateFunds(quint64 funds) {
    _fundsItem->setText(fundsToString(funds));
}

void ChannelView::updateRefundLockTime(quint32 refundLockTime) {
    _refundLockTimeItem->setText(refundLockTimeToString(refundLockTime));
}

void ChannelView::updatePrice(quint64 price) {
    _priceItem->setText(priceToString(price));
}

void ChannelView::updateNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentMadeItem->setText(numberOfPaymentsMadeToString(numberOfPaymentsMade));
}

void ChannelView::updateBalance(quint64 balance) {
    _balanceItem->setText(balanceToString(balance));
}
