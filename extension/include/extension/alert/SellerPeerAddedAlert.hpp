/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_SELLER_PEER_PLUGIN_ADDED_ALERT_HPP
#define EXTENSION_SELLER_PEER_PLUGIN_ADDED_ALERT_HPP

#include <extension/alert/TorrentPluginAlert.hpp>
#include <extension/alert/AlertTypes.hpp>
#include <extension/SellerPeerPluginStatus.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class SellerPeerAddedAlert : public TorrentPluginAlert {

    public:

        // Public member required for alert_cast
        const static int alert_type = SELLER_PEER_PLUGIN_ADDED_ALERT_ID;

        // Constructor from members
        SellerPeerAddedAlert(const libtorrent::sha1_hash & infoHash,
                            const libtorrent::tcp::endpoint & endPoint,
                            const SellerPeerPluginStatus & status);

        // Copy Constructor
        SellerPeerAddedAlert(const SellerPeerAddedAlert & alert);

        // Virtual routines from libtrorrent::alert
        virtual int type() const;
        virtual char const* what() const;
        virtual std::string message() const;
        virtual int category() const;
        virtual std::auto_ptr<alert> clone() const;

        // Getters and setters
        libtorrent::tcp::endpoint endPoint() const;
        void setEndPoint(const libtorrent::tcp::endpoint & endPoint);

        SellerPeerPluginStatus status() const;
        void setStatus(const SellerPeerPluginStatus & status);

    private:

        // Endpoint for peer started
        libtorrent::tcp::endpoint _endPoint;

        // Status for started peer
        SellerPeerPluginStatus _status;
    };

}
}
}

#endif // EXTENSION_SELLER_PEER_PLUGIN_ADDED_ALERT_HPP
