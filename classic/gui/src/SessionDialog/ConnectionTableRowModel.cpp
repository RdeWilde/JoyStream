/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#include <gui/SessionDialog/ConnectionTableRowModel.hpp>
#include <gui/Common.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

ConnectionTableRowModel::ConnectionTableRowModel(QObject * parent,
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

int ConnectionTableRowModel::row() const noexcept {
    return _hostItem->row();
}

void ConnectionTableRowModel::setHost(const libtorrent::tcp::endpoint & endPoint) {
    std::string hostString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(hostString));
}

void ConnectionTableRowModel::setState(const core::CBStateMachine::InnerStateIndex & stateIndex) {

    if(stateIndex.is_initialized())
        _stateItem->setText(Common::toString(stateIndex.get()));
    else
        _stateItem->setText("-");
}

void ConnectionTableRowModel::setFunds(quint64 funds) {
    _fundsItem->setText(BitcoinRepresentation(funds).toString(_settings));
}

void ConnectionTableRowModel::setRefundLockTime(quint64 refundLockTime) {
    _lockItem->setText(QString::number(refundLockTime));
}

void ConnectionTableRowModel::setPrice(quint64 price) {
    _priceItem->setText(BitcoinRepresentation(price).toString(_settings));
}

void ConnectionTableRowModel::setNumberOfPayments(quint64 numberOfPayments) {
    _numberOfPaymentsItem->setText(QString::number(numberOfPayments));
}

void ConnectionTableRowModel::setBalance(qint64 balance) {
    _balanceItem->setText(BitcoinRepresentation(balance).toString(_settings));
}

}
}
}
