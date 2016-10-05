/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#include <gui/PeersDialog/BuyerTableModel.hpp>
#include <gui/PeersDialog/BuyerTableRowModel.hpp>

#include <gui/Common.hpp>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace classic {
namespace gui {

BuyerTableModel::BuyerTableModel(BitcoinDisplaySettings * settings)
    : _model(0, 5)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << QObject::tr("Host")
                << QObject::tr("Max. Price")
                << QObject::tr("Max. Lock")
                << QObject::tr("Min. #sellers")
                << QObject::tr("Max. Contract fee/Kb");

    _model.setHorizontalHeaderLabels(columnNames);

    // Center text
    for(int i = 0; i < 5;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
}

BuyerTableRowModel * BuyerTableModel::add(const libtorrent::tcp::endpoint & endPoint) {

    // Create items
    QStandardItem * hostItem = new QStandardItem(),
                  * maxPriceItem = new QStandardItem(),
                  * maxLockItem = new QStandardItem(),
                  * minNumberOfSellersItem = new QStandardItem(),
                  * maxContractFeePerKbItem = new QStandardItem();

    // Make endpoint recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(endPoint);
    hostItem->setData(var, Qt::UserRole);

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    maxPriceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    maxLockItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    minNumberOfSellersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    maxContractFeePerKbItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    maxPriceItem->setEditable(false);
    maxLockItem->setEditable(false);
    minNumberOfSellersItem->setEditable(false);
    maxContractFeePerKbItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << maxPriceItem
          << maxLockItem
          << minNumberOfSellersItem
          << maxContractFeePerKbItem;

    _model.appendRow(items);

    // Create row model
    return new BuyerTableRowModel(hostItem,
                                  maxPriceItem,
                                  maxLockItem,
                                  minNumberOfSellersItem,
                                  maxContractFeePerKbItem,
                                  _settings);
}

libtorrent::tcp::endpoint BuyerTableModel::endPoint(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::tcp::endpoint>(_model, row, 0);
}

QAbstractItemModel * BuyerTableModel::model() noexcept {
    return &_model;
}

}
}
}
