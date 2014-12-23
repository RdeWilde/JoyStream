#ifndef START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP
#define START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

class StartPluginTorrentPluginRequest : public TorrentPluginRequest
{

public:
    StartPluginTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

    // Getters
    const TorrentPluginConfiguration * getTorrentPluginConfiguration() const;

private:

    // Configuration
    // We do not own this, recipient of configuration must delete.
    TorrentPluginConfiguration * _torrentPluginConfiguration;
};

#endif // START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP
