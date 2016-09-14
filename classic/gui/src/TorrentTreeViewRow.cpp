/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/TorrentTreeViewRow.hpp>
#include <gui/Language.hpp>
#include <core/core.hpp>
#include <common/DataSizeRepresentation.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

TorrentTreeViewRow::TorrentTreeViewRow(const BitcoinDisplaySettings * settings,
                                       QStandardItem * nameItem,
                                       QStandardItem * sizeItem,
                                       QStandardItem * stateItem,
                                       QStandardItem * uploadSpeed,
                                       QStandardItem * downloadSpeed,
                                       QStandardItem * numberOfBuyerPeersItem,
                                       QStandardItem * numberOfSellerPeersitem,
                                       QStandardItem * sessionModeItem,
                                       QStandardItem * balanceItem)
    : _settings(settings)
    , _nameItem(nameItem)
    , _sizeItem(sizeItem)
    , _stateItem(stateItem)
    , _uploadSpeedItem(uploadSpeed)
    , _downloadSpeedItem(downloadSpeed)
    , _numberOfBuyerPeersItem(numberOfBuyerPeersItem)
    , _numberOfSellerPeersitem(numberOfSellerPeersitem)
    , _sessionModeItem(sessionModeItem)
    , _balanceItem(balanceItem) {
}

QStandardItem * TorrentTreeViewRow::nameItem() const noexcept{
    return _nameItem;
}

QStandardItem * TorrentTreeViewRow::sizeItem() const noexcept{
    return _sizeItem;
}

QStandardItem * TorrentTreeViewRow::stateItem() const noexcept{
    return _stateItem;
}

QStandardItem * TorrentTreeViewRow::uploadSpeedItem() const noexcept{
    return _uploadSpeedItem;
}

QStandardItem * TorrentTreeViewRow::downloadSpeedItem() const noexcept{
    return _downloadSpeedItem;
}

QStandardItem * TorrentTreeViewRow::numberOfBuyerPeersItem() const noexcept{
    return _numberOfBuyerPeersItem;
}

QStandardItem * TorrentTreeViewRow::numberOfSellerPeersitem() const noexcept{
    return _numberOfSellerPeersitem;
}

QStandardItem * TorrentTreeViewRow::sessionModeItem() const noexcept{
    return _sessionModeItem;
}

QStandardItem * TorrentTreeViewRow::balanceItem() const noexcept {
    return _balanceItem;
}

void TorrentTreeViewRow::setName(const std::string & name) {
    _nameItem->setText(QString::fromStdString(name));
}

void TorrentTreeViewRow::setSize(qint64 totalSize) {
    _sizeItem->setText(DataSizeRepresentation(totalSize, DataSizeRepresentation::Base::Byte).toString());
}

void TorrentTreeViewRow::setState(libtorrent::torrent_status::state_t state, float progress) {
    _stateItem->setText(Language::toString(state, progress));
}

void TorrentTreeViewRow::setPaused(bool paused) {

    if(paused)
        _stateItem->setText(QObject::tr("Paused"));
    else
        _stateItem->setText(QObject::tr("Resuming")); // we must get old state refershed, so lets just wait for new state update
}

void TorrentTreeViewRow::setUploadSpeed(int speed) {
    _uploadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTreeViewRow::setDownloadSpeed(int speed) {
    _downloadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTreeViewRow::setNumberOfBuyers(quint32 num) {
    _numberOfBuyerPeersItem->setText(QString::number(num));
}

void TorrentTreeViewRow::setNumberOfSellers(quint32 num) {
    _numberOfSellerPeersitem->setText(QString::number(num));
}

void TorrentTreeViewRow::setSessionMode(protocol_session::SessionMode mode) {
    _sessionModeItem->setText(Language::toString(mode));
}

void TorrentTreeViewRow::setBalance(qint64 balance) {
    _balanceItem->setText(BitcoinRepresentation(balance).toString(_settings));
}

}
}
}
