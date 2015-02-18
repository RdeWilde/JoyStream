#ifndef START_SELLER_TORRENT_PLUGIN_HPP
#define START_SELLER_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include "extension/SellerTorrentPlugin.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash

class StartSellerTorrentPlugin : public PluginRequest {

public:

    // Constructor
    StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash,
                             const SellerTorrentPlugin::Configuration & configuration);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash &infoHash);

    SellerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const SellerTorrentPlugin::Configuration &configuration);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Configuration
    SellerTorrentPlugin::Configuration _configuration;
};

#endif // START_SELLER_TORRENT_PLUGIN_HPP
