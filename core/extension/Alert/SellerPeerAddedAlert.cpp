/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "SellerPeerAddedAlert.hpp"

SellerPeerAddedAlert::SellerPeerAddedAlert(const libtorrent::sha1_hash & infoHash,
                                           const libtorrent::tcp::endpoint & endPoint,
                                           const SellerPeerPlugin::Status & status)
    : TorrentPluginAlert(infoHash)
    , _endPoint(endPoint)
    , _status(status) {
}

SellerPeerAddedAlert::SellerPeerAddedAlert(const SellerPeerAddedAlert & alert)
    : TorrentPluginAlert(alert)
    , _endPoint(alert.endPoint())
    , _status(alert.status()) {
}

int SellerPeerAddedAlert::type() const {
    return alert_type;
}

char const * SellerPeerAddedAlert::what() const {
    return "SellerPeerAddedAlert";
}

std::string SellerPeerAddedAlert::message() const {
    return std::string("SellerPeerAddedAlert::message: IMPLEMENT LATER");
}

int SellerPeerAddedAlert::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> SellerPeerAddedAlert::clone() const {
    return std::auto_ptr<libtorrent::alert>(new SellerPeerAddedAlert(*this));
}

libtorrent::tcp::endpoint SellerPeerAddedAlert::endPoint() const {
    return _endPoint;
}

void SellerPeerAddedAlert::setEndPoint(const libtorrent::tcp::endpoint & endPoint) {
    _endPoint = endPoint;
}

SellerPeerPlugin::Status SellerPeerAddedAlert::status() const {
    return _status;
}

void SellerPeerAddedAlert::setStatus(const SellerPeerPlugin::Status & status) {
    _status = status;
}
