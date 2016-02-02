/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/alert/SellerTorrentPluginStatusAlert.hpp>

namespace joystream {
namespace extension {
namespace alert {

    SellerTorrentPluginStatusAlert::SellerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPluginStatus status)
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

    std::auto_ptr<libtorrent::alert> SellerTorrentPluginStatusAlert::clone() const {
        return std::auto_ptr<libtorrent::alert>(new SellerTorrentPluginStatusAlert(*this));
    }

    SellerTorrentPluginStatus SellerTorrentPluginStatusAlert::status() const {
        return _status;
    }

    void SellerTorrentPluginStatusAlert::setStatus(const SellerTorrentPluginStatus & status) {
        _status = status;
    }

}
}
}
