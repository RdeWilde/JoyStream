#ifndef SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP
#define SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/PluginMode.hpp"

class SetPluginModeTorrentPluginRequest : public TorrentPluginRequest
{
public:

    // Constructor
    SetPluginModeTorrentPluginRequest(libtorrent::sha1_hash & info_hash, PluginMode pluginMode);

    // Getters
    PluginMode getPluginMode() const;

private:

    // Mode
    PluginMode _pluginMode;

};

#endif // SET_PLUGIN_MODE_TORRENT_PLUGIN_REQUEST_HPP
