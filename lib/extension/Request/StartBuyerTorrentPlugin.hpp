#ifndef START_BUYER_TORRENT_PLUGIN_HPP
#define START_BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/BuyerTorrentPluginConfiguration.hpp"

class StartBuyerTorrentPlugin : public TorrentPluginRequest {

public:

    // Constructor
    StartBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPluginConfiguration & configuration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

    // Getters
    BuyerTorrentPluginConfiguration getConfiguration() const;

private:

    // Configuration
    BuyerTorrentPluginConfiguration _configuration;
};

#endif // START_BUYER_TORRENT_PLUGIN_HPP
