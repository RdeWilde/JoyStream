/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#include <gui/MainWindow/TorrentTableModel.hpp>
#include <gui/MainWindow/TorrentTableRowModel.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <gui/Common.hpp>
Q_DECLARE_METATYPE(libtorrent::sha1_hash)

namespace joystream {
namespace classic {
namespace gui {

TorrentTableModel::TorrentTableModel(BitcoinDisplaySettings * settings)
    : _model(0, 9)
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

    _model.setHorizontalHeaderLabels(columnNames);

    for(int i = 0; i < 9;i++)
        _model.setHeaderData(i, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);

}

TorrentTableRowModel * TorrentTableModel::add(const libtorrent::sha1_hash & infoHash) noexcept {

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

    // Make info hash recoverable from name item, is used to do reverse lookup
    QVariant var;
    var.setValue(infoHash);
    nameItem->setData(var, Qt::UserRole);

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

    _model.appendRow(items);

    // Create and return row
    return new TorrentTableRowModel(&_model,
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
}

libtorrent::sha1_hash TorrentTableModel::infoHash(int row) const {
    return gui::Common::getUserRoleDataFromTableModel<libtorrent::sha1_hash>(_model, row, 0);
}


QAbstractItemModel * TorrentTableModel::model() noexcept {
    return &_model;
}

}
}
}
