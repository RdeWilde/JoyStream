/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/MainWindow/TorrentTableRowModel.hpp>
#include <gui/Common.hpp>
#include <common/DataSizeRepresentation.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

TorrentTableRowModel::TorrentTableRowModel(QObject * parent,
                                           QStandardItem * nameItem,
                                           QStandardItem * sizeItem,
                                           QStandardItem * stateItem,
                                           QStandardItem * uploadSpeed,
                                           QStandardItem * downloadSpeed,
                                           QStandardItem * numberOfBuyerPeersItem,
                                           QStandardItem * numberOfSellerPeersitem,
                                           QStandardItem * sessionModeItem,
                                           QStandardItem * balanceItem,
                                           const BitcoinDisplaySettings * settings)
    : QObject(parent)
    , _nameItem(nameItem)
    , _sizeItem(sizeItem)
    , _stateItem(stateItem)
    , _uploadSpeedItem(uploadSpeed)
    , _downloadSpeedItem(downloadSpeed)
    , _numberOfBuyerPeersItem(numberOfBuyerPeersItem)
    , _numberOfSellerPeersitem(numberOfSellerPeersitem)
    , _sessionModeItem(sessionModeItem)
    , _balanceItem(balanceItem)
    , _settings(settings) {
}

void TorrentTableRowModel::setName(const std::string & name) {
    _nameItem->setText(QString::fromStdString(name));
}

void TorrentTableRowModel::setSize(qint64 totalSize) {
    _sizeItem->setText(DataSizeRepresentation(totalSize, DataSizeRepresentation::Base::Byte).toString());
}

void TorrentTableRowModel::setState(libtorrent::torrent_status::state_t state, float progress) {
    _stateItem->setText(Common::toString(state, progress));
}

void TorrentTableRowModel::setPaused(bool paused) {

    if(paused)
        _stateItem->setText(QObject::tr("Paused"));
    else
        _stateItem->setText(QObject::tr("Resuming")); // we must get old state refershed, so lets just wait for new state update
}

void TorrentTableRowModel::setUploadSpeed(int speed) {
    _uploadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTableRowModel::setDownloadSpeed(int speed) {
    _downloadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTableRowModel::setNumberOfBuyers(quint32 num) {
    _numberOfBuyerPeersItem->setText(QString::number(num));
}

void TorrentTableRowModel::setNumberOfSellers(quint32 num) {
    _numberOfSellerPeersitem->setText(QString::number(num));
}

void TorrentTableRowModel::setSessionMode(protocol_session::SessionMode mode) {
    _sessionModeItem->setText(Common::toString(mode));
}

void TorrentTableRowModel::setBalance(qint64 balance) {
    _balanceItem->setText(BitcoinRepresentation(balance).toString(_settings));
}

void TorrentTableRowModel::unsetTorrentPluginPresence() {
    _numberOfBuyerPeersItem->setText("-");
    _numberOfSellerPeersitem->setText("-");
    _sessionModeItem->setText("-");
    _balanceItem->setText("-");
}

int TorrentTableRowModel::row() const noexcept {
    return _nameItem->row();
}

}
}
}
