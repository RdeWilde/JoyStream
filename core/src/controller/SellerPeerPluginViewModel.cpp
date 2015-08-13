/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/SellerPeerPluginViewModel.hpp>
#include <core/controller/SellerTorrentPluginViewModel.hpp>

#include <QStandardItem>

SellerPeerPluginViewModel::SellerPeerPluginViewModel(QObject * parent,
                                                     const libtorrent::tcp::endpoint & endPoint,
                                                     const SellerPeerPlugin::Status & status)
    : QObject(parent)
    , _endPoint(endPoint)
    , _clientState(status.clientState())
    , _payeeViewModel(this, status.payeeStatus()) {
}

void SellerPeerPluginViewModel::update(const SellerPeerPlugin::Status & status) {

    if(_clientState != status.clientState()) {
        _clientState = status.clientState();
        emit clientStateChanged(status.clientState());
    }

    _payeeViewModel.update(status.payeeStatus());
}

libtorrent::tcp::endpoint SellerPeerPluginViewModel::endPoint() const {
    return _endPoint;
}

SellerPeerPlugin::ClientState SellerPeerPluginViewModel::clientState() const {
    return _clientState;
}

const PayeeViewModel * SellerPeerPluginViewModel::payeeViewModel() const {
    return &_payeeViewModel;
}

/**
QList<int> SellerPeerPluginViewModel::fullPiecesSent() const {
    return _fullPiecesSent;
}
*/
