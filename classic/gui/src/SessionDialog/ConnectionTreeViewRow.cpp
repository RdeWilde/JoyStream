/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#include <gui/SessionDialog/ConnectionTreeViewRow.hpp>
#include <gui/Common.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

ConnectionTreeViewRow::ConnectionTreeViewRow(QObject * parent,
                                             QStandardItem * hostItem,
                                             QStandardItem * stateItem,
                                             QStandardItem * fundsItem,
                                             QStandardItem * lockItem,
                                             QStandardItem * priceItem,
                                             QStandardItem * numberOfPaymentsItem,
                                             QStandardItem * balanceItem,
                                             const BitcoinDisplaySettings * settings)
    : QObject(parent)
    , _hostItem(hostItem)
    , _stateItem(stateItem)
    , _fundsItem(fundsItem)
    , _lockItem(lockItem)
    , _priceItem(priceItem)
    , _numberOfPaymentsItem(numberOfPaymentsItem)
    , _balanceItem(balanceItem)
    , _settings(settings) {
}

QStandardItem * ConnectionTreeViewRow::hostItem() const noexcept {
    return _hostItem;
}

QStandardItem * ConnectionTreeViewRow::stateItem() const noexcept {
    return _stateItem;
}

QStandardItem * ConnectionTreeViewRow::fundsItem() const noexcept {
    return _fundsItem;
}

QStandardItem * ConnectionTreeViewRow::lockItem() const noexcept {
    return _lockItem;
}

QStandardItem * ConnectionTreeViewRow::priceItem() const noexcept {
    return _priceItem;
}

QStandardItem * ConnectionTreeViewRow::numberOfPayments() const noexcept {
    return _numberOfPaymentsItem;
}

QStandardItem * ConnectionTreeViewRow::balance() const noexcept {
    return _balanceItem;
}

int ConnectionTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

void ConnectionTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    std::string hostString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(hostString));
}

void ConnectionTreeViewRow::setState(const core::CBStateMachine::InnerStateIndex & stateIndex) {

    if(stateIndex.is_initialized())
        _stateItem->setText(Common::toString(stateIndex.get()));
    else
        _stateItem->setText("-");
}

void ConnectionTreeViewRow::setFunds(quint64 funds) {
    _fundsItem->setText(BitcoinRepresentation(funds).toString(_settings));
}

void ConnectionTreeViewRow::setRefundLockTime(quint64 refundLockTime) {
    _lockItem->setText(QString::number(refundLockTime));
}

void ConnectionTreeViewRow::setPrice(quint64 price) {
    _priceItem->setText(BitcoinRepresentation(price).toString(_settings));
}

void ConnectionTreeViewRow::setNumberOfPayments(quint64 numberOfPayments) {
    _numberOfPaymentsItem->setText(QString::number(numberOfPayments));
}

void ConnectionTreeViewRow::setBalance(qint64 balance) {
    _balanceItem->setText(BitcoinRepresentation(balance).toString(_settings));
}

}
}
}
