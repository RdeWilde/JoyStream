/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef START_SELLER_TORRENT_PLUGIN_HPP
#define START_SELLER_TORRENT_PLUGIN_HPP

#include <core/extension/Request/PluginRequest.hpp>
#include <core/extension/SellerTorrentPlugin.hpp>
#include <libtorrent/sha1_hash.hpp>

class StartSellerTorrentPlugin : public PluginRequest
{
public:

    // Constructor from members
    StartSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters and setters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash &infoHash);

    SellerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const SellerTorrentPlugin::Configuration & configuration);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Seller torrent plugin configuration
    SellerTorrentPlugin::Configuration _configuration;
};

#endif // START_SELLER_TORRENT_PLUGIN_HPP
