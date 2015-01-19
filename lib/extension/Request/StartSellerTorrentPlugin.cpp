#include "StartSellerTorrentPlugin.hpp"

StartSellerTorrentPlugin::StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPluginConfiguration & configuration)
    : _info_hash(info_hash)
    , _configuration(configuration) {
}

PluginRequestType StartSellerTorrentPlugin::getTorrentPluginRequestType() const {
    return PluginRequestType::StartSellerTorrentPlugin;
}

SellerTorrentPluginConfiguration StartSellerTorrentPlugin::getConfiguration() const {
    return _configuration;
}
