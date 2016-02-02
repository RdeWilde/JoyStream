/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/request/StartBuyerTorrentPlugin.hpp>

namespace joystream {
namespace extension {
namespace request {

    StartBuyerTorrentPlugin::StartBuyerTorrentPlugin() {
    }

    StartBuyerTorrentPlugin::StartBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPluginConfiguration configuration, const Coin::UnspentP2PKHOutput & utxo)
        : _infoHash(infoHash)
        , _configuration(configuration)
        , _utxo(utxo) {
    }

    PluginRequestType StartBuyerTorrentPlugin::getPluginRequestType() const {
        return PluginRequestType::StartBuyerTorrentPlugin;
    }

    libtorrent::sha1_hash StartBuyerTorrentPlugin::infoHash() const {
        return _infoHash;
    }

    void StartBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
        _infoHash = infoHash;
    }

    BuyerTorrentPluginConfiguration StartBuyerTorrentPlugin::configuration() const {
        return _configuration;
    }

    void StartBuyerTorrentPlugin::setConfiguration(const BuyerTorrentPluginConfiguration & configuration) {
        _configuration = configuration;
    }

    Coin::UnspentP2PKHOutput StartBuyerTorrentPlugin::utxo() const {
        return _utxo;
    }

    void StartBuyerTorrentPlugin::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
        _utxo = utxo;
    }

}
}
}
