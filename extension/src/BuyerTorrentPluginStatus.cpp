/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#include <extension/status/BuyerTorrentPlugin.hpp>

namespace joystream {
namespace extension {
namespace status {

    BuyerTorrentPlugin::BuyerTorrentPlugin() {
    }

    BuyerTorrentPlugin::BuyerTorrentPlugin(BuyerTorrentPlugin::State state,
                                       const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                                       const Payor::Status & payor)
        : _state(state)
        , _peerPluginStatuses(peers)
        , _payor(payor) {
    }

    BuyerTorrentPlugin::State BuyerTorrentPlugin::state() const {
        return _state;
    }

    void BuyerTorrentPlugin::setState(State state) {
        _state = state;
    }

    QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> BuyerTorrentPlugin::peerPluginStatuses() const {
        return _peerPluginStatuses;
    }

    void BuyerTorrentPlugin::setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> & peerPluginStatuses) {
        _peerPluginStatuses = peerPluginStatuses;
    }

    Payor::Status BuyerTorrentPlugin::payor() const {
        return _payor;
    }

    void BuyerTorrentPlugin::setPayor(const Payor::Status & payor) {
        _payor = payor;
    }


}
}
}
