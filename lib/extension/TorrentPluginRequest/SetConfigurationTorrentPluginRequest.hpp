#ifndef SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP
#define SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

class SetConfigurationTorrentPluginRequest : public TorrentPluginRequest {

public:

    // Configuration
    TorrentPluginConfiguration _torrentPluginConfiguration;

    // Constructor
    SetConfigurationTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, const TorrentPluginConfiguration & torrentPluginConfiguration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;
};

#endif // SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP
