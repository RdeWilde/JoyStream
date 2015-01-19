#ifndef START_SELLER_TORRENT_PLUGIN_HPP
#define START_SELLER_TORRENT_PLUGIN_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/SellerTorrentPluginConfiguration.hpp"

class StartSellerTorrentPlugin : public TorrentPluginRequest
{
public:

    // Constructor
    StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPluginConfiguration & configuration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

    // Getters
    SellerTorrentPluginConfiguration getConfiguration() const;

private:

    // Configuration
    SellerTorrentPluginConfiguration _configuration;
};

#endif // START_SELLER_TORRENT_PLUGIN_HPP
