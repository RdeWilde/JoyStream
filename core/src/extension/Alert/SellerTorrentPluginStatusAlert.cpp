/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/SellerTorrentPluginStatusAlert.hpp>

SellerTorrentPluginStatusAlert::SellerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status status)
    : TorrentPluginAlert(infoHash)
    , _status(status) {
}

SellerTorrentPluginStatusAlert::SellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert & alert)
    : TorrentPluginAlert(alert)
    , _status(alert.status()) {
}

int SellerTorrentPluginStatusAlert::type() const {
    return alert_type;
}

char const* SellerTorrentPluginStatusAlert::what() const {
    return "SellerTorrentPluginStatusAlert";
}

std::string SellerTorrentPluginStatusAlert::message() const {
    return std::string("SellerTorrentPluginStatusAlert::message: IMPLEMENT LATER");
}

int SellerTorrentPluginStatusAlert::category() const {
    return libtorrent::alert::stats_notification;
}

std::auto_ptr<libtorrent::alert> SellerTorrentPluginStatusAlert::clone_impl() const {
    return std::auto_ptr<libtorrent::alert>(new SellerTorrentPluginStatusAlert(*this));
}

SellerTorrentPlugin::Status SellerTorrentPluginStatusAlert::status() const {
    return _status;
}

void SellerTorrentPluginStatusAlert::setStatus(const SellerTorrentPlugin::Status & status) {
    _status = status;
}
