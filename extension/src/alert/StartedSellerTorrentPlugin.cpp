/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/alert/StartedSellerTorrentPlugin.hpp>

namespace joystream {
namespace extension {
namespace alert {

    StartedSellerTorrentPlugin::StartedSellerTorrentPlugin() {
    }

    StartedSellerTorrentPlugin::StartedSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                                                           const SellerTorrentPluginConfiguration & configuration,
                                                           const SellerTorrentPluginStatus status)
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

    SellerTorrentPluginConfiguration StartedSellerTorrentPlugin::configuration() const {
        return _configuration;
    }

    void StartedSellerTorrentPlugin::setConfiguration(const SellerTorrentPluginConfiguration & configuration) {
        _configuration = configuration;
    }

    SellerTorrentPluginStatus StartedSellerTorrentPlugin::status() const {
        return _status;
    }

    void StartedSellerTorrentPlugin::setStatus(const SellerTorrentPluginStatus & status) {
        _status = status;
    }

}
}
}
