/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/alert/BuyerPeerAddedAlert.hpp>

namespace joystream {
namespace extension {
namespace alert {

    BuyerPeerAddedAlert::BuyerPeerAddedAlert(const libtorrent::sha1_hash & infoHash,
                                             const libtorrent::tcp::endpoint & endPoint,
                                             const BuyerPeerPluginStatus & status)
        : TorrentPluginAlert(infoHash)
        , _endPoint(endPoint)
        , _status(status) {
    }

    BuyerPeerAddedAlert::BuyerPeerAddedAlert(const BuyerPeerAddedAlert & alert)
        : TorrentPluginAlert(alert)
        , _endPoint(alert.endPoint())
        , _status(alert.status()) {
    }

    int BuyerPeerAddedAlert::type() const {
        return alert_type;
    }

    char const * BuyerPeerAddedAlert::what() const {
        return "BuyerPeerAddedAlert";
    }

    std::string BuyerPeerAddedAlert::message() const {
        return std::string("BuyerPeerAddedAlert::message: IMPLEMENT LATER");
    }

    int BuyerPeerAddedAlert::category() const {
        return libtorrent::alert::status_notification;
    }

    std::auto_ptr<libtorrent::alert> BuyerPeerAddedAlert::clone() const {
        return std::auto_ptr<libtorrent::alert>(new BuyerPeerAddedAlert(*this));
    }

    libtorrent::tcp::endpoint BuyerPeerAddedAlert::endPoint() const {
        return _endPoint;
    }

    /**
    void BuyerPeerAddedAlert::setEndPoint(const libtorrent::tcp::endpoint & endPoint) {
        _endPoint = endPoint;
    }
    */

    BuyerPeerPluginStatus BuyerPeerAddedAlert::status() const {
        return _status;
    }

    /**
    void BuyerPeerAddedAlert::setStatus(const BuyerPeerPlugin::Status & status) {
        _status = status;
    }
    */

}
}
}
