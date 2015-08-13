/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_PEER_PLUGIN_REMOVED_ALERT_HPP
#define SELLER_PEER_PLUGIN_REMOVED_ALERT_HPP

#include <core/extension/Alert/TorrentPluginAlert.hpp>
#include <core/extension/Alert/AlertTypes.hpp>
#include <core/extension/SellerPeerPlugin.hpp>

class SellerPeerPluginRemovedAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = SELLER_PEER_PLUGIN_REMOVED_ALERT_ID;

    // Constructor from members
    SellerPeerPluginRemovedAlert(const libtorrent::sha1_hash & infoHash,
                                 const libtorrent::tcp::endpoint & endPoint);

    // Copy Constructor
    SellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert & alert);

    // Virtual routines from libtrorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    libtorrent::tcp::endpoint endPoint() const;

private:

    // Endpoint for peer started
    libtorrent::tcp::endpoint _endPoint;

    // SOME SORT OF REASON HERE?
    // ** if nothing meaningful beyond a simple
    // Seller peer plugin status is required, then
    // this alert becomes identical with the added
    // alert, in which case one can just combined them
    // and add some information about if peer is being added
    // or removed


private:
};

#endif // SELLER_PEER_PLUGIN_REMOVED_ALERT_HPP
