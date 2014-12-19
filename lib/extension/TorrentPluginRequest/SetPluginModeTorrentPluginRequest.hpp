#ifndef SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP
#define SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/StartedPluginMode.hpp"

class SetPluginModeTorrentPluginRequest : public TorrentPluginRequest
{
public:

    // Constructor
    SetPluginModeTorrentPluginRequest(libtorrent::sha1_hash & info_hash, StartedPluginMode pluginMode);

    // Getters
    StartedPluginMode getStartedPluginMode() const;

private:

    // Mode
    StartedPluginMode _pluginMode;

};

#endif // SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP
