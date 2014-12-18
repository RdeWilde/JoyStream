#include "SetConfigurationTorrentPluginRequest.hpp"

SetConfigurationTorrentPluginRequest::SetConfigurationTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration)
    : TorrentPluginRequest(info_hash)
    , _torrentPluginConfiguration(torrentPluginConfiguration) {
}

TorrentPluginRequestType SetConfigurationTorrentPluginRequest::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::SetConfiguration;
}

TorrentPluginConfiguration * SetConfigurationTorrentPluginRequest::getTorrentPluginConfiguration() {
    return _torrentPluginConfiguration;
}
