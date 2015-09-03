/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/TorrentPluginStartedAlert.hpp>

TorrentPluginStartedAlert::TorrentPluginStartedAlert(const libtorrent::sha1_hash & infoHash, PluginMode mode)
    : _infoHash(infoHash)
    , _mode(mode) {
}

TorrentPluginStartedAlert::TorrentPluginStartedAlert(const TorrentPluginStartedAlert & alert)
    : _infoHash(alert.infoHash())
    , _mode(alert.mode()){
}

int TorrentPluginStartedAlert::type() const {
    return alert_type;
}

char const* TorrentPluginStartedAlert::what() const {
    return "TorrentPluginStartedAlert";
}

std::string TorrentPluginStartedAlert::message() const {
    return std::string("TorrentPluginStartedAlert::message: IMPLEMENT LATER");
}

int TorrentPluginStartedAlert::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> TorrentPluginStartedAlert::clone() const {
    return std::auto_ptr<alert>(new TorrentPluginStartedAlert(*this));
}

libtorrent::sha1_hash TorrentPluginStartedAlert::infoHash() const {
    return _infoHash;
}

void TorrentPluginStartedAlert::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

PluginMode TorrentPluginStartedAlert::mode() const {
    return _mode;
}

void TorrentPluginStartedAlert::setMode(PluginMode mode){
    _mode = mode;
}
