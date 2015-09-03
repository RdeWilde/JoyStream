/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/StartedSellerTorrentPlugin.hpp>

StartedSellerTorrentPlugin::StartedSellerTorrentPlugin() {
}

StartedSellerTorrentPlugin::StartedSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                                                       const SellerTorrentPlugin::Configuration & configuration,
                                                       const SellerTorrentPlugin::Status status)
    : _infoHash(infoHash)
    , _configuration(configuration)
    , _status(status) {
}

StartedSellerTorrentPlugin::StartedSellerTorrentPlugin(const StartedSellerTorrentPlugin & alert)
    : _infoHash(alert.infoHash())
    , _configuration(alert.configuration())
    , _status(alert.status()){
}

int StartedSellerTorrentPlugin::type() const {
    return alert_type;
}

char const* StartedSellerTorrentPlugin::what() const {
    return "StartedSellerTorrentPlugin";
}

std::string StartedSellerTorrentPlugin::message() const {
    return std::string("StartedSellerTorrentPlugin::message: IMPLEMENT LATER");
}

int StartedSellerTorrentPlugin::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> StartedSellerTorrentPlugin::clone() const {
    return std::auto_ptr<libtorrent::alert>(new StartedSellerTorrentPlugin(*this));
}

libtorrent::sha1_hash StartedSellerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartedSellerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

SellerTorrentPlugin::Configuration StartedSellerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartedSellerTorrentPlugin::setConfiguration(const SellerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}

SellerTorrentPlugin::Status StartedSellerTorrentPlugin::status() const {
    return _status;
}

void StartedSellerTorrentPlugin::setStatus(const SellerTorrentPlugin::Status & status) {
    _status = status;
}

