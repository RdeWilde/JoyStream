/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 17 2016
 */

#include <gui/PeersDialog/PeerTreeViewRow.hpp>
#include <gui/Common.hpp>
#include <extension/extension.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

PeerTreeViewRow::PeerTreeViewRow(QStandardItem * host,
                             QStandardItem * clientName,
                             QStandardItem * BEPSupportStatus)
    : _hostItem(host)
    , _clientNameItem(clientName)
    , _BEPSupportStatusItem(BEPSupportStatus) {
}

QStandardItem * PeerTreeViewRow::hostItem() const noexcept {
    return _hostItem;
}

QStandardItem * PeerTreeViewRow::clientNameItem() const noexcept {
    return _clientNameItem;
}

QStandardItem * PeerTreeViewRow::BEPSupportStatusItem() const noexcept {
    return _BEPSupportStatusItem;
}

void PeerTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

void PeerTreeViewRow::setClientName(const std::string & clientName) {
    _clientNameItem->setText(QString::fromStdString(clientName));
}

void PeerTreeViewRow::setBEPSupport(const extension::BEPSupportStatus & status) {
    _BEPSupportStatusItem->setText(Common::toString(status));
}

int PeerTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

}
}
}
