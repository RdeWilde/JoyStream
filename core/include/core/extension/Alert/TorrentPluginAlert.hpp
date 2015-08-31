/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef TORRENT_PLUGIN_ALERT_HPP
#define TORRENT_PLUGIN_ALERT_HPP

#include <libtorrent/alert.hpp>
#include <libtorrent/sha1_hash.hpp>

// Abstract type for all torrent plugin alerts
class TorrentPluginAlert : public libtorrent::alert {

public:

    // Constructor from members
    TorrentPluginAlert(const libtorrent::sha1_hash & infoHash);

    // Copy constructor
    TorrentPluginAlert(const TorrentPluginAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const = 0;
    virtual char const* what() const = 0;
    virtual std::string message() const = 0;
    virtual int category() const = 0;
    virtual std::auto_ptr<alert> clone_impl() const = 0;

    // Getters
    libtorrent::sha1_hash infoHash() const;

protected:

    libtorrent::sha1_hash _infoHash;
};

#endif // TORRENT_PLUGIN_ALERT_HPP
