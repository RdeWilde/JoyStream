/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#include <extension/BuyerTorrentPluginStatus.hpp>

namespace joystream {
namespace extension {

    BuyerTorrentPluginStatus::BuyerTorrentPluginStatus() {
    }

    BuyerTorrentPluginStatus::BuyerTorrentPluginStatus(BuyerTorrentPluginState state,
                                       const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> & peers,
                                       const PayorStatus & payor)
        : _state(state)
        , _peerPluginStatuses(peers)
        , _payor(payor) {
    }

    BuyerTorrentPluginState BuyerTorrentPluginStatus::state() const {
        return _state;
    }

    void BuyerTorrentPluginStatus::setState(BuyerTorrentPluginState state) {
        _state = state;
    }

    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> BuyerTorrentPluginStatus::peerPluginStatuses() const {
        return _peerPluginStatuses;
    }

    void BuyerTorrentPluginStatus::setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> & peerPluginStatuses) {
        _peerPluginStatuses = peerPluginStatuses;
    }

    PayorStatus BuyerTorrentPluginStatus::payor() const {
        return _payor;
    }

    void BuyerTorrentPluginStatus::setPayor(const PayorStatus & payor) {
        _payor = payor;
    }


}
}
