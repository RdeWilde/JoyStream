#include "SetPluginModeTorrentPluginRequest.hpp"

SetPluginModeTorrentPluginRequest::SetPluginModeTorrentPluginRequest(libtorrent::sha1_hash & info_hash, PluginMode pluginMode)
    : TorrentPluginRequest(info_hash)
    , _pluginMode(pluginMode) {
}

PluginMode SetPluginModeTorrentPluginRequest::getPluginMode() const {
    return _pluginMode;
}
