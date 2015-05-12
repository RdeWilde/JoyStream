#include "ChannelView.hpp"

ChannelView::ChannelView(QObject *parent)
    : QObject(parent) {

}

QString ChannelView::stateToString(Payor::Channel::State state) const {

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

QString ChannelView::fundsToString(quint64 funds) const {
    return QString::number(funds);
}

QString ChannelView::refundLockTimeToString(quint32 refundLockTime) const {
    return QString::number(refundLockTime);
}

QString ChannelView::priceToString(quint64 price) const {
    return QString::number(price);
}

QString ChannelView::numberOfPaymentsMadeToString(quint64 numberOfPaymentsMade) const {
    return QString::number(numberOfPaymentsMade);
}

QString ChannelView::balanceToString(quint64 balance) const {
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
