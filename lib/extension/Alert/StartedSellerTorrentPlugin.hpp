/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef STARTED_SELLER_TORRENT_PLUGIN_HPP
#define STARTED_SELLER_TORRENT_PLUGIN_HPP

#include "AlertTypes.hpp"

#include "extension/SellerTorrentPlugin.hpp"

#include <libtorrent/alert.hpp>
#include <libtorrent/sha1_hash.hpp>

class StartedSellerTorrentPlugin : public libtorrent::alert
{
public:

    // Public member required for alert_cast
    const static int alert_type = STARTED_SELLER_TORRENT_PLUGIN_ID;

    // Default constructor
    StartedSellerTorrentPlugin();

    // Constructor from members
    StartedSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                               const SellerTorrentPlugin::Configuration & configuration,
                               const SellerTorrentPlugin::Status status);

    // Copy constructor
    StartedSellerTorrentPlugin(const StartedSellerTorrentPlugin & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    SellerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const SellerTorrentPlugin::Configuration & configuration);

    SellerTorrentPlugin::Status status() const;
    void setStatus(const SellerTorrentPlugin::Status & status);

private:

    // Info hash of torrent started
    libtorrent::sha1_hash _infoHash;

    // Configuration under which plugin was started
    SellerTorrentPlugin::Configuration _configuration;

    // Starting status
    SellerTorrentPlugin::Status _status;
};

#endif // STARTED_SELLER_TORRENT_PLUGIN_HPP
