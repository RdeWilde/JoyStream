/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#include <gui/PeersDialog/PeerTableModel.hpp>
#include <gui/PeersDialog/PeerTableRowModel.hpp>

namespace joystream {
namespace classic {
namespace gui {

PeerTableModel::PeerTableModel(BitcoinDisplaySettings * settings)
    : _standardItemModel(0, 3)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << QObject::tr("Host")
                << QObject::tr("Client")
                << QObject::tr("Extension");

    _standardItemModel.setHorizontalHeaderLabels(columnNames);
}

PeerTableModel::~PeerTableModel() {

    for(auto mapping : _rowModels)
        delete mapping.second;
}

PeerTableRowModel * PeerTableModel::add(const libtorrent::tcp::endpoint & endPoint) {

    if(_rowModels.count(endPoint) > 0)
        throw std::runtime_error("Torrent already added.");

    // Create items
    QStandardItem * hostItem = new QStandardItem(),
                  * clientItem = new QStandardItem(),
                  * extensionItem = new QStandardItem();

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

    _standardItemModel.appendRow(items);

    // Create row model
    PeerTableRowModel * rowModel = new PeerTableRowModel(hostItem,
                                                         clientItem,
                                                         extensionItem);

    // Add to row model mapping
    _rowModels[endPoint] = rowModel;

    return rowModel;
}

void PeerTableModel::remove(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _rowModels.find(endPoint);

    if(it == _rowModels.cend())
        throw std::runtime_error("No such torrent exists.");
    else {

        PeerTableRowModel * rowModel = it->second;

        // Remove from standard model
        _standardItemModel.removeRow(rowModel->row());

        // Delete row model
        delete rowModel;

        // Remove from mapping
        _rowModels.erase(it);

    }
}

std::map<libtorrent::tcp::endpoint, PeerTableRowModel *> PeerTableModel::rowModels() const noexcept {
    return _rowModels;
}

QStandardItemModel * PeerTableModel::standardItemModel() noexcept {
    return &_standardItemModel;
}

}
}
}
