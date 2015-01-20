#ifndef START_BUYER_TORRENT_PLUGIN_HPP
#define START_BUYER_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include "extension/BuyerTorrentPluginConfiguration.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash

class StartBuyerTorrentPlugin : public PluginRequest {

public:

    // Constructor
    StartBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPluginConfiguration & configuration);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters

    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash &infoHash);

    BuyerTorrentPluginConfiguration configuration() const;
    void setConfiguration(const BuyerTorrentPluginConfiguration &configuration);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Configuration
    BuyerTorrentPluginConfiguration _configuration;
};

#endif // START_BUYER_TORRENT_PLUGIN_HPP
