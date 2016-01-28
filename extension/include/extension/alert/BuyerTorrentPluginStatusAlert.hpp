/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
#define BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP

#include <core/extension/Alert/TorrentPluginAlert.hpp>
#include <core/extension/Alert/AlertTypes.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp>

class BuyerTorrentPluginStatusAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = BUYER_TORRENT_PLUGIN_STATUS_ALERT_ID;

    // Constructor from members
    BuyerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status);

    // Copy constructor
    BuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<alert> clone() const;

    // Getters and Setters
    BuyerTorrentPlugin::Status status() const;
    void setStatus(const BuyerTorrentPlugin::Status & status);

private:

    // Plugin status
    BuyerTorrentPlugin::Status _status;
};

#endif // BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
