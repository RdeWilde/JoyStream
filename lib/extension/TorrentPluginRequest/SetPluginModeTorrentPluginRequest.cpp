#include "SetPluginModeTorrentPluginRequest.hpp"

SetPluginModeTorrentPluginRequest::SetPluginModeTorrentPluginRequest(libtorrent::sha1_hash & info_hash, PluginMode pluginMode)
    : TorrentPluginRequest(info_hash)
    , _pluginMode(pluginMode) {
}

PluginMode SetPluginModeTorrentPluginRequest::getStartedPluginMode() const {
    return _pluginMode;
}
