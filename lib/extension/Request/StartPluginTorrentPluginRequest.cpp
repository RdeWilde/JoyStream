#include "StartPluginTorrentPluginRequest.hpp"

StartPluginTorrentPluginRequest::StartPluginTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration)
    : TorrentPluginRequest(info_hash)
    , _torrentPluginConfiguration(torrentPluginConfiguration) {
}

TorrentPluginRequestType StartPluginTorrentPluginRequest::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::StartPlugin;
}

const TorrentPluginConfiguration * StartPluginTorrentPluginRequest::getTorrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}
