/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#include <gui/PeersDialog/PeerTableModel.hpp>
#include <gui/PeersDialog/PeerTableRowModel.hpp>

#include <gui/Common.hpp>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace classic {
namespace gui {

PeerTableModel::PeerTableModel(BitcoinDisplaySettings * settings)
    : _model(0, 3)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << QObject::tr("Host")
                << QObject::tr("Client")
                << QObject::tr("Extension");

    _model.setHorizontalHeaderLabels(columnNames);

    // Center text
    for(int i = 0; i < 3;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
}

PeerTableRowModel * PeerTableModel::add(const libtorrent::tcp::endpoint & endPoint) noexcept {

    // Create items
    QStandardItem * hostItem = new QStandardItem(),
                  * clientItem = new QStandardItem(),
                  * extensionItem = new QStandardItem();

    // Make info hash recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(endPoint);
    hostItem->setData(var, Qt::UserRole);

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    clientItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    extensionItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    clientItem->setEditable(false);
    extensionItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << clientItem
          << extensionItem;

    _model.appendRow(items);

    // Create row model
    return new PeerTableRowModel(&_model,
                                 hostItem,
                                 clientItem,
                                 extensionItem);
}

libtorrent::tcp::endpoint PeerTableModel::endPoint(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::tcp::endpoint>(_model, row, 0);
}

QAbstractItemModel * PeerTableModel::model() noexcept {
    return &_model;
}

}
}
}
