/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 3 2016
 */

#include <gui/SessionDialog/ConnectionTableModel.hpp>
#include <gui/SessionDialog/ConnectionTableRowModel.hpp>

#include <gui/Common.hpp>
// not neede due to core inclusion in hpp, which is temproray: Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace classic {
namespace gui {

ConnectionTableModel::ConnectionTableModel(BitcoinDisplaySettings * settings)
    : _model(0, 7)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Host"
                << "State"
                << "Funds"
                << "Lock Time"
                << "Price"
                << "#Payments"
                << "Balance";

    _model.setHorizontalHeaderLabels(columnNames);

    for(int i = 0; i < 7;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);

}

ConnectionTableRowModel * ConnectionTableModel::add(const libtorrent::tcp::endpoint & endPoint) noexcept {

    // Create items
    QStandardItem * hostItem  = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * fundsItem = new QStandardItem(),
                  * lockTimeItem = new QStandardItem(),
                  * priceItem = new QStandardItem(),
                  * numberOfPaymentsItem = new QStandardItem(),
                  * balanceItem = new QStandardItem();

    // Make info hash recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(endPoint);
    hostItem->setData(var, Qt::UserRole);

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    fundsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    lockTimeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    priceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfPaymentsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    stateItem->setEditable(false);
    fundsItem->setEditable(false);
    lockTimeItem->setEditable(false);
    priceItem->setEditable(false);
    numberOfPaymentsItem->setEditable(false);
    balanceItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << stateItem
          << fundsItem
          << lockTimeItem
          << priceItem
          << numberOfPaymentsItem
          << balanceItem;

    _model.appendRow(items);

    // Create and return row
    return new ConnectionTableRowModel(&_model,
                                       hostItem,
                                       stateItem,
                                       fundsItem,
                                       lockTimeItem,
                                       priceItem,
                                       numberOfPaymentsItem,
                                       balanceItem,
                                       _settings);
}

libtorrent::tcp::endpoint ConnectionTableModel::endPoint(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::tcp::endpoint>(_model, row, 0);
}

QAbstractItemModel * ConnectionTableModel::model() noexcept {
    return &_model;
}

}
}
}
