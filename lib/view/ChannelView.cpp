#include "ChannelView.hpp"

#include <QStandardItem>
#include <QStandardItemModel>

ChannelView::ChannelView(QStandardItemModel * model,
                         Payor::Channel::State state,
                         quint64 funds,
                         quint32 refundLockTime,
                         quint64 price,
                         quint64 numberOfPaymentsMade,
                         quint64 balance)
    : _stateItem(new QStandardItem(stateToString(state)))
    , _fundsItem(new QStandardItem(fundsToString(funds)))
    , _refundLockTimeItem(new QStandardItem(refundLockTimeToString(refundLockTime)))
    , _priceItem(new QStandardItem(priceToString(price)))
    , _numberOfPaymentMadeItem(new QStandardItem(numberOfPaymentsMadeToString(numberOfPaymentsMade)))
    , _balanceItem(new QStandardItem(balanceToString(balance))) {

    // Add row to model
    QList<QStandardItem *> items;

    items << _stateItem
          << _fundsItem
          << _refundLockTimeItem
          << _priceItem
          << _numberOfPaymentMadeItem
          << _balanceItem;

    model->appendRow(items);

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
