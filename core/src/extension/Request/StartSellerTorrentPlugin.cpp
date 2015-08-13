/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Request/StartSellerTorrentPlugin.hpp>

StartSellerTorrentPlugin::StartSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration)
    : _infoHash(infoHash)
    , _configuration(configuration) {
}

// Returns request type
PluginRequestType StartSellerTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartSellerTorrentPlugin;
}

libtorrent::sha1_hash StartSellerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartSellerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

SellerTorrentPlugin::Configuration StartSellerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartSellerTorrentPlugin::setConfiguration(const SellerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}


