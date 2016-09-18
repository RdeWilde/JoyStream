/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <gui/PeersDialog/SellerConnectionTreeViewRow.hpp>
#include <gui/Common.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

SellerConnectionTreeViewRow::SellerConnectionTreeViewRow(QStandardItem * hostItem,
                                                         QStandardItem * minPriceItem,
                                                         QStandardItem * minLockItem,
                                                         QStandardItem * maxSellersItem,
                                                         QStandardItem * minContractFeePerKbItem,
                                                         QStandardItem * settlementFeeItem,
                                                         const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _minPriceItem(minPriceItem)
    , _minLockItem(minLockItem)
    , _maxSellersItem(maxSellersItem)
    , _minContractFeePerKbItem(minContractFeePerKbItem)
    , _settlementFeeItem(settlementFeeItem)
    , _settings(settings) {

}

QStandardItem * SellerConnectionTreeViewRow::hostItem() const noexcept {
    return _hostItem;
}

QStandardItem * SellerConnectionTreeViewRow::minPriceItem() const noexcept {
    return _minPriceItem;
}

QStandardItem * SellerConnectionTreeViewRow::minLockItem() const noexcept {
    return _minLockItem;
}

QStandardItem * SellerConnectionTreeViewRow::maxSellersItem() const noexcept {
    return _maxSellersItem;
}

QStandardItem * SellerConnectionTreeViewRow::minContractFeePerKbItem() const noexcept {
    return _minContractFeePerKbItem;
}

QStandardItem * SellerConnectionTreeViewRow::settlementFeeItem() const noexcept {
    return _settlementFeeItem;
}

void SellerConnectionTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

void SellerConnectionTreeViewRow::setTerms(const protocol_wire::SellerTerms & terms) {

    _minPriceItem->setText(BitcoinRepresentation(terms.minPrice()).toString(_settings));
    _minLockItem->setText(QString::number(terms.minLock()));
    _maxSellersItem->setText(QString::number(terms.maxSellers()));
    _minContractFeePerKbItem->setText(BitcoinRepresentation(terms.minContractFeePerKb()).toString(_settings));
    _settlementFeeItem->setText(BitcoinRepresentation(terms.settlementFee()).toString(_settings));
}

int SellerConnectionTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

}
}
}
