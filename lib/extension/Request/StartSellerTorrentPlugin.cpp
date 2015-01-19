#include "StartSellerTorrentPlugin.hpp"

StartSellerTorrentPlugin::StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPluginConfiguration & configuration)
    : TorrentPluginRequest(info_hash)
    , _configuration(configuration) {
}

TorrentPluginRequestType StartSellerTorrentPlugin::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::StartSellerTorrentPlugin;
}

SellerTorrentPluginConfiguration StartSellerTorrentPlugin::getConfiguration() const {
    return _configuration;
}
