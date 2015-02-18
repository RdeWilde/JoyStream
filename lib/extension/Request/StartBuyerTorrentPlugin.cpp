#include "StartBuyerTorrentPlugin.hpp"

StartBuyerTorrentPlugin::StartBuyerTorrentPlugin(const libtorrent::sha1_hash &info_hash,
                                                 const BuyerTorrentPlugin::Configuration & configuration)
    : _infoHash(info_hash)
    , _configuration(configuration) {
}

PluginRequestType StartBuyerTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartBuyerTorrentPlugin;
}

libtorrent::sha1_hash StartBuyerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash &infoHash) {
    _infoHash = infoHash;
}

BuyerTorrentPlugin::Configuration StartBuyerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartBuyerTorrentPlugin::setConfiguration(const BuyerTorrentPlugin::Configuration &configuration) {
    _configuration = configuration;
}

