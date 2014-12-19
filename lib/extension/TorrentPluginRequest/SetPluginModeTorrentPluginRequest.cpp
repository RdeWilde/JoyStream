#include "SetPluginModeTorrentPluginRequest.hpp"

SetPluginModeTorrentPluginRequest::SetPluginModeTorrentPluginRequest(libtorrent::sha1_hash & info_hash, StartedPluginMode pluginMode)
    : TorrentPluginRequest(info_hash)
    , _pluginMode(pluginMode) {
}

StartedPluginMode SetPluginModeTorrentPluginRequest::getStartedPluginMode() const {
    return _pluginMode;
}
