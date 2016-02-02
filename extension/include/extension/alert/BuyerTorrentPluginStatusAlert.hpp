/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
#define EXTENSION_BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP

#include <extension/alert/TorrentPluginAlert.hpp>
#include <extension/alert/AlertTypes.hpp>
#include <extension/BuyerTorrentPluginStatus.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class BuyerTorrentPluginStatusAlert : public TorrentPluginAlert {

    public:

        // Public member required for alert_cast
        const static int alert_type = BUYER_TORRENT_PLUGIN_STATUS_ALERT_ID;

        // Constructor from members
        BuyerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPluginStatus & status);

        // Copy constructor
        BuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert & alert);

        // Virtual routines from libtorrent::alert
        virtual int type() const;
        virtual char const* what() const;
        virtual std::string message() const;
        virtual int category() const;
        virtual std::auto_ptr<alert> clone() const;

        // Getters and Setters
        BuyerTorrentPluginStatus status() const;
        void setStatus(const BuyerTorrentPluginStatus & status);

    private:

        // Plugin status
        BuyerTorrentPluginStatus _status;
    };

}
}
}

#endif // EXTENSION_BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
