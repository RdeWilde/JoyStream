/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP
#define EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP

#include <extension/SellerTorrentPluginStatus.hpp>
#include <extension/alert/TorrentPluginAlert.hpp>
#include <extension/alert/AlertTypes.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class SellerTorrentPluginStatusAlert : public TorrentPluginAlert {
    public:

        // Public member required for alert_cast
        const static int alert_type = SELLER_TORRENT_PLUGIN_STATUS_ALERT_ID;

        // Constructor from members
        SellerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPluginStatus status);

        // Copy constructor
        SellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert & alert);

        // Virtual routines from libtorrent::alert
        virtual int type() const;
        virtual char const* what() const;
        virtual std::string message() const;
        virtual int category() const;
        virtual std::auto_ptr<libtorrent::alert> clone() const;

        // Getters and Setters
        SellerTorrentPluginStatus status() const;
        void setStatus(const SellerTorrentPluginStatus & status);

    private:

        // Plugin status
        SellerTorrentPluginStatus _status;
    };

}
}
}

#endif // EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_ALERT_HPP
