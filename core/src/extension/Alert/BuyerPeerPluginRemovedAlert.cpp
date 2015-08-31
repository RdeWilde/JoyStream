/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/BuyerPeerPluginRemovedAlert.hpp>

BuyerPeerPluginRemovedAlert::BuyerPeerPluginRemovedAlert(const libtorrent::sha1_hash & infoHash,
                                                         const libtorrent::tcp::endpoint & endPoint)
    : TorrentPluginAlert(infoHash)
    , _endPoint(endPoint) {

}

BuyerPeerPluginRemovedAlert::BuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert & alert)
    : BuyerPeerPluginRemovedAlert(alert.infoHash(), alert.endPoint()) {
}

int BuyerPeerPluginRemovedAlert::type() const {
    return alert_type;
}

char const* BuyerPeerPluginRemovedAlert::what() const {
    return "BuyerPeerPluginRemovedAlert";
}

std::string BuyerPeerPluginRemovedAlert::message() const {
    return std::string("BuyerPeerPluginRemovedAlert::message: IMPLEMENT LATER");
}

int BuyerPeerPluginRemovedAlert::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> BuyerPeerPluginRemovedAlert::clone_impl() const {
    return std::auto_ptr<libtorrent::alert>(new BuyerPeerPluginRemovedAlert(*this));
}

libtorrent::tcp::endpoint BuyerPeerPluginRemovedAlert::endPoint() const {
    return _endPoint;
}
