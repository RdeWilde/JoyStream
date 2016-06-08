/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_START_SELLER_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_REQUEST_START_SELLER_TORRENT_PLUGIN_HPP

#include <extension/request/PluginRequest.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace extension {
namespace request {

    class StartSellerTorrentPlugin : public PluginRequest {

    public:

        // Constructor from members
        StartSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPluginConfiguration & configuration);

        // Returns request type
        virtual PluginRequestType getPluginRequestType() const;

        // Getters and setters
        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash &infoHash);

        SellerTorrentPluginConfiguration configuration() const;
        void setConfiguration(const SellerTorrentPluginConfiguration & configuration);

    private:

        // Torrent info_hash
        libtorrent::sha1_hash _infoHash;

        // Seller torrent plugin configuration
        SellerTorrentPluginConfiguration _configuration;
    };

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_START_SELLER_TORRENT_PLUGIN_HPP
