#include "StartBuyerTorrentPlugin.hpp"

StartBuyerTorrentPlugin::StartBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPluginConfiguration & configuration)
    : TorrentPluginRequest(info_hash)
    , _configuration(configuration) {
}

TorrentPluginRequestType StartBuyerTorrentPlugin::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::StartBuyerTorrentPlugin;
}

BuyerTorrentPluginConfiguration StartBuyerTorrentPlugin::getConfiguration() const {
    return _configuration;
}
