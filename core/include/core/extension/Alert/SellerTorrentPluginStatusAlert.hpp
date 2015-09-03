/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP
#define SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP

#include <core/extension/Alert/TorrentPluginAlert.hpp>
#include <core/extension/Alert/AlertTypes.hpp>
#include <core/extension/SellerTorrentPlugin.hpp>

class SellerTorrentPluginStatusAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = SELLER_TORRENT_PLUGIN_STATUS_ALERT_ID;

    // Constructor from members
    SellerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status status);

    // Copy constructor
    SellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and Setters
    SellerTorrentPlugin::Status status() const;
    void setStatus(const SellerTorrentPlugin::Status & status);

private:

    // Plugin status
    SellerTorrentPlugin::Status _status;
};

#endif // SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP
