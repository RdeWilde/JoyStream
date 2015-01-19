#ifndef START_SELLER_TORRENT_PLUGIN_HPP
#define START_SELLER_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include "extension/SellerTorrentPluginConfiguration.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash

class StartSellerTorrentPlugin : public PluginRequest {

public:

    // Constructor
    StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPluginConfiguration & configuration);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    SellerTorrentPluginConfiguration getConfiguration() const;

private:

    // Torrent info_hash
    libtorrent::sha1_hash _info_hash;

    // Configuration
    SellerTorrentPluginConfiguration _configuration;
};

#endif // START_SELLER_TORRENT_PLUGIN_HPP
