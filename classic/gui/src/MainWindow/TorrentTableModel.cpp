/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#include <gui/MainWindow/TorrentTableModel.hpp>
#include <gui/MainWindow/TorrentTableRowModel.hpp>

#include <common/BitcoinDisplaySettings.hpp>

namespace joystream {
namespace classic {
namespace gui {

TorrentTableModel::TorrentTableModel(BitcoinDisplaySettings * settings)
    : _standardItemModel(0, 9)
    , _settings(settings) {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Name"
                << "Size"
                << "State"
                << "Upload Speed"
                << "Download Speed"
                << "#Buyers"
                << "#Sellers"
                << "Mode"
                << "Balance";

    _standardItemModel.setHorizontalHeaderLabels(columnNames);
}

TorrentTableModel::~TorrentTableModel() {

    for(auto mapping : _rowModels)
        delete mapping.second;
}

TorrentTableRowModel * TorrentTableModel::add(const libtorrent::tcp::endpoint & endPoint) {

    if(_rowModels.count(endPoint) > 0)
        throw std::runtime_error("Torrent already added.");

    // Create items
    QStandardItem * nameItem  = new QStandardItem(),
                  * sizeItem = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * uploadSpeedItem = new QStandardItem(),
                  * downloadSpeedItem = new QStandardItem(),
                  * numberOfBuyerPeersItem = new QStandardItem(),
                  * numberOfSellerPeersitem = new QStandardItem(),
                  * sessionModeItem = new QStandardItem(),
                  * balanceItem = new QStandardItem();

    // Center content
    nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    sizeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    uploadSpeedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    downloadSpeedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfBuyerPeersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfSellerPeersitem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    sessionModeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    nameItem->setEditable(false);
    sizeItem->setEditable(false);
    stateItem->setEditable(false);
    uploadSpeedItem->setEditable(false);
    downloadSpeedItem->setEditable(false);
    numberOfBuyerPeersItem->setEditable(false);
    numberOfSellerPeersitem->setEditable(false);
    sessionModeItem->setEditable(false);
    balanceItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << nameItem
          << sizeItem
          << stateItem
          << uploadSpeedItem
          << downloadSpeedItem
          << numberOfBuyerPeersItem
          << numberOfSellerPeersitem
          << sessionModeItem
          << balanceItem;

    _standardItemModel.appendRow(items);

    // Create row model
    TorrentTableRowModel * rowModel = new TorrentTableRowModel(nullptr,
                                                               nameItem,
                                                               sizeItem,
                                                               stateItem,
                                                               uploadSpeedItem,
                                                               downloadSpeedItem,
                                                               numberOfBuyerPeersItem,
                                                               numberOfSellerPeersitem,
                                                               sessionModeItem,
                                                               balanceItem,
                                                               _settings);

    // Add to row model mapping
    _rowModels[endPoint] = rowModel;

    return rowModel;
}

void TorrentTableModel::remove(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _rowModels.find(endPoint);

    if(it == _rowModels.cend())
        throw std::runtime_error("No such torrent exists.");
    else {

        TorrentTableRowModel * rowModel = it->second;

        // Remove from standard model
        _standardItemModel.removeRow(rowModel->row());

        // Delete row model
        delete rowModel;

        // Remove from mapping
        _rowModels.erase(it);

    }
}

std::map<libtorrent::tcp::endpoint, TorrentTableRowModel *> TorrentTableModel::rowModels() const noexcept {
    return _rowModels;
}

QStandardItemModel * TorrentTableModel::standardItemModel() noexcept {
    return &_standardItemModel;
}

}
}
}
