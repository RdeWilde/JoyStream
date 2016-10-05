/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#include <gui/PeersDialog/SellerTableModel.hpp>
#include <gui/PeersDialog/SellerTableRowModel.hpp>

#include <gui/Common.hpp>

Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace classic {
namespace gui {

SellerTableModel::SellerTableModel(const BitcoinDisplaySettings * settings)
    : _model(0, 6)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << QObject::tr("Host")
                << QObject::tr("Min. Price")
                << QObject::tr("Min. Lock")
                << QObject::tr("Max. #sellers")
                << QObject::tr("Max. Contract fee/Kb")
                << QObject::tr("Settlement fee");

    _model.setHorizontalHeaderLabels(columnNames);

    // Center text
    for(int i = 0; i < 6;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
}

SellerTableRowModel * SellerTableModel::add(const libtorrent::tcp::endpoint & endPoint) noexcept {

    // Create items
    QStandardItem * hostItem = new QStandardItem(),
                  * minPriceItem = new QStandardItem(),
                  * minLockItem = new QStandardItem(),
                  * maxSellersItem = new QStandardItem(),
                  * minContractFeePerKbItem= new QStandardItem(),
                  * settlementFeeItem = new QStandardItem();

    // Make info hash recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(endPoint);
    hostItem->setData(var, Qt::UserRole);

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    minPriceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    minLockItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    maxSellersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    minContractFeePerKbItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    settlementFeeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    minPriceItem->setEditable(false);
    minLockItem->setEditable(false);
    maxSellersItem->setEditable(false);
    minContractFeePerKbItem->setEditable(false);
    settlementFeeItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << minPriceItem
          << minLockItem
          << maxSellersItem
          << minContractFeePerKbItem
          << settlementFeeItem;

    _model.appendRow(items);

    // Create row model
    return new SellerTableRowModel(&_model,
                                   hostItem,
                                   minPriceItem,
                                   minLockItem,
                                   maxSellersItem,
                                   minContractFeePerKbItem,
                                   settlementFeeItem,
                                   _settings);
}

libtorrent::tcp::endpoint SellerTableModel::endPoint(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::tcp::endpoint>(_model, row, 0);
}

QAbstractItemModel * SellerTableModel::model() noexcept {
    return &_model;
}

}
}
}
