/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_ALERT_BUYER_PEER_ADDED_ALERT_HPP
#define EXTENSION_ALERT_BUYER_PEER_ADDED_ALERT_HPP

#include <extension/alert/TorrentPluginAlert.hpp>
#include <extension/alert/AlertTypes.hpp>
#include <extension/BuyerPeerPluginStatus.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class BuyerPeerAddedAlert : public TorrentPluginAlert {

    public:

        // Public member required for alert_cast
        const static int alert_type = BUYER_PEER_PLUGIN_ADDED_ALERT_ID;

        // Constructor from members
        BuyerPeerAddedAlert(const libtorrent::sha1_hash & infoHash,
                            const libtorrent::tcp::endpoint & endPoint,
                            const BuyerPeerPluginStatus & status);

        // Copy Constructor
        BuyerPeerAddedAlert(const BuyerPeerAddedAlert & alert);

        // Virtual routines from libtrorrent::alert
        virtual int type() const;
        virtual char const* what() const;
        virtual std::string message() const;
        virtual int category() const;
        virtual std::auto_ptr<alert> clone() const;

        // Getters and setters
        libtorrent::tcp::endpoint endPoint() const;
        //void setEndPoint(const libtorrent::tcp::endpoint &endPoint);

        BuyerPeerPluginStatus status() const;
        //void setStatus(const BuyerPeerPlugin::Status &status);

    private:

        // Endpoint for peer started
        libtorrent::tcp::endpoint _endPoint;

        // Status for started peer
        BuyerPeerPluginStatus _status;

    };

}
}
}

#endif // EXTENSION_ALERT_BUYER_PEER_ADDED_ALERT_HPP
