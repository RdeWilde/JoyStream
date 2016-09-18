/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <gui/PeersDialog/BuyerConnectionTreeViewRow.hpp>
#include <gui/Common.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

BuyerConnectionTreeViewRow::BuyerConnectionTreeViewRow(QStandardItem * hostItem,
                                                       QStandardItem * maxPriceItem,
                                                       QStandardItem * maxLockItem,
                                                       QStandardItem * minNumberOfSellersItem,
                                                       QStandardItem * maxContractFeePerKbItem,
                                                       const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _maxPriceItem(maxPriceItem)
    , _maxLockItem(maxLockItem)
    , _minNumberOfSellersItem(minNumberOfSellersItem)
    , _maxContractFeePerKbItem(maxContractFeePerKbItem)
    , _settings(settings) {
}

QStandardItem * BuyerConnectionTreeViewRow::hostItem() const noexcept {
    return _hostItem;
}

QStandardItem * BuyerConnectionTreeViewRow::maxPriceItem() const noexcept {
    return _maxPriceItem;
}

QStandardItem * BuyerConnectionTreeViewRow::maxLockItem() const noexcept {
    return _maxLockItem;
}

QStandardItem * BuyerConnectionTreeViewRow::minNumberOfSellersItem() const noexcept {
    return _minNumberOfSellersItem;
}

QStandardItem * BuyerConnectionTreeViewRow::maxContractFeePerKbItem() const noexcept {
    return _maxContractFeePerKbItem;
}

int BuyerConnectionTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

void BuyerConnectionTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

void BuyerConnectionTreeViewRow::setTerms(const protocol_wire::BuyerTerms & terms) {

    _maxPriceItem->setText(BitcoinRepresentation(terms.maxPrice()).toString(_settings));
    _maxLockItem->setText(QString::number(terms.maxLock()));
    _minNumberOfSellersItem->setText(QString::number(terms.minNumberOfSellers()));
    _maxContractFeePerKbItem->setText(BitcoinRepresentation(terms.maxContractFeePerKb()).toString(_settings));
}

}
}
}
