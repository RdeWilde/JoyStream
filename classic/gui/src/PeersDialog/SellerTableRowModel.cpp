/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <gui/PeersDialog/SellerTableRowModel.hpp>
#include <gui/PeersDialog/SellerTableModel.hpp>
#include <gui/Common.hpp>

#include <protocol_wire/protocol_wire.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <libtorrent/socket_io.hpp>

namespace joystream {
namespace classic {
namespace gui {

SellerTableRowModel::SellerTableRowModel(QObject * parent,
                                         QStandardItem * hostItem,
                                         QStandardItem * minPriceItem,
                                         QStandardItem * minLockItem,
                                         QStandardItem * maxSellersItem,
                                         QStandardItem * minContractFeePerKbItem,
                                         QStandardItem * settlementFeeItem,
                                         const BitcoinDisplaySettings * settings)
    : QObject(parent)
    , _hostItem(hostItem)
    , _minPriceItem(minPriceItem)
    , _minLockItem(minLockItem)
    , _maxSellersItem(maxSellersItem)
    , _minContractFeePerKbItem(minContractFeePerKbItem)
    , _settlementFeeItem(settlementFeeItem)
    , _settings(settings) {
}

void SellerTableRowModel::setHost(const libtorrent::tcp::endpoint & endPoint) noexcept {
    _hostItem->setText(Common::toString(endPoint));
}

void SellerTableRowModel::setTerms(const protocol_wire::SellerTerms & terms) noexcept {

    _minPriceItem->setText(BitcoinRepresentation(terms.minPrice()).toString(_settings));
    _minLockItem->setText(QString::number(terms.minLock()));
    _maxSellersItem->setText(QString::number(terms.maxSellers()));
    _minContractFeePerKbItem->setText(BitcoinRepresentation(terms.minContractFeePerKb()).toString(_settings));
    _settlementFeeItem->setText(BitcoinRepresentation(terms.settlementFee()).toString(_settings));
}

}
}
}
