#ifndef SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP
#define SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

class SetConfigurationTorrentPluginRequest : public TorrentPluginRequest {

public:

    // Constructor
    SetConfigurationTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

    // Getters
    TorrentPluginConfiguration * getTorrentPluginConfiguration();

private:

    // Configuration
    TorrentPluginConfiguration * _torrentPluginConfiguration;
};

#endif // SET_CONFIGURATION_TORRENT_PLUGIN_REQUEST_HPP
