/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 17 2016
 */

#include <gui/PeersDialog/PeerTableRowModel.hpp>
#include <gui/Common.hpp>
#include <extension/extension.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

PeerTableRowModel::PeerTableRowModel(QObject * parent,
                                     QStandardItem * host,
                                     QStandardItem * clientName,
                                     QStandardItem * BEPSupportStatus)
    : QObject(parent)
    , _hostItem(host)
    , _clientNameItem(clientName)
    , _BEPSupportStatusItem(BEPSupportStatus) {
}

void PeerTableRowModel::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

void PeerTableRowModel::setClientName(const std::string & clientName) {
    _clientNameItem->setText(QString::fromStdString(clientName));
}

void PeerTableRowModel::setBEPSupport(const extension::BEPSupportStatus & status) {
    _BEPSupportStatusItem->setText(Common::toString(status));
}

int PeerTableRowModel::row() const noexcept {
    return _hostItem->row();
}

}
}
}
