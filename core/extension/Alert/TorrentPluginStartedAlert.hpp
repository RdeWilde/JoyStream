/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef TORRENT_PLUGIN_STARTED_ALERT_HPP
#define TORRENT_PLUGIN_STARTED_ALERT_HPP

#include "AlertTypes.hpp"
#include "extension/PluginMode.hpp"

#include <libtorrent/alert.hpp>
#include <libtorrent/sha1_hash.hpp>

class TorrentPluginStartedAlert : public libtorrent::alert
{
public:

    // Public member required for alert_cast
    const static int alert_type = TORRENT_PLUGIN_STARTED_ALERT_ID;

    // Constructor from members
    TorrentPluginStartedAlert(const libtorrent::sha1_hash & infoHash, PluginMode mode);

    // Copy constructor
    TorrentPluginStartedAlert(const TorrentPluginStartedAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    PluginMode mode() const;
    void setMode(PluginMode mode);

private:

    // Info hash of torrent started
    libtorrent::sha1_hash _infoHash;

    // Mode of started torrent plugin
    PluginMode _mode;
};

#endif // TORRENT_PLUGIN_STARTED_ALERT_HPP
