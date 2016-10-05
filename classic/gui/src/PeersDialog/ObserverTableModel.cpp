/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#include <gui/PeersDialog/ObserverTableModel.hpp>
#include <gui/PeersDialog/ObserverTableRowModel.hpp>

#include <gui/Common.hpp>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace classic {
namespace gui {

ObserverTableModel::ObserverTableModel(BitcoinDisplaySettings * settings)
    : _model(0, 1)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << QObject::tr("Host");

    _model.setHorizontalHeaderLabels(columnNames);

    // Center text
    for(int i = 0; i < 1;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
}

ObserverTableRowModel * ObserverTableModel::add(const libtorrent::tcp::endpoint & endPoint) {

    // Create items
    QStandardItem * hostItem = new QStandardItem();

    // Make info hash recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(endPoint);
    hostItem->setData(var, Qt::UserRole);

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem;

    _model.appendRow(items);

    // Create row model
    return new ObserverTableRowModel(hostItem, _settings);
}

libtorrent::tcp::endpoint ObserverTableModel::endPoint(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::tcp::endpoint>(_model, row, 0);
}

QAbstractItemModel * ObserverTableModel::model() noexcept {
    return &_model;
}

}
}
}
