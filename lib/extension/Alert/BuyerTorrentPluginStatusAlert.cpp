/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "BuyerTorrentPluginStatusAlert.hpp"


BuyerTorrentPluginStatusAlert::BuyerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status)
    : TorrentPluginAlert(infoHash)
    , _status(status){
}

BuyerTorrentPluginStatusAlert::BuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert & alert)
    : TorrentPluginAlert(alert)
    , _status(alert.status()){
}

int BuyerTorrentPluginStatusAlert::type() const {
    return alert_type;
}

char const * BuyerTorrentPluginStatusAlert::what() const {
    return "BuyerTorrentPluginStatusAlert";
}

std::string BuyerTorrentPluginStatusAlert::message() const {
    return std::string("BuyerTorrentPluginStatusAlert::message: IMPLEMENT LATER");
}

int BuyerTorrentPluginStatusAlert::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> BuyerTorrentPluginStatusAlert::clone() const {
    return std::auto_ptr<alert>(new BuyerTorrentPluginStatusAlert(*this));
}

BuyerTorrentPlugin::Status BuyerTorrentPluginStatusAlert::status() const {
    return _status;
}

void BuyerTorrentPluginStatusAlert::setStatus(const BuyerTorrentPlugin::Status &status) {
    _status = status;
}
