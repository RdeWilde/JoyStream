#include "StartBuyerTorrentPlugin.hpp"

StartBuyerTorrentPlugin::StartBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPluginConfiguration & configuration)
    : _infoHash(info_hash)
    , _configuration(configuration) {
}

PluginRequestType StartBuyerTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartBuyerTorrentPlugin;
}

BuyerTorrentPluginConfiguration StartBuyerTorrentPlugin::getConfiguration() const {
    return _configuration;
}

libtorrent::sha1_hash StartBuyerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash &infoHash) {
    _infoHash = infoHash;
}

libtorrent::sha1_hash StartBuyerTorrentPlugin::info_hash() const {
    return _infoHash;
}

void StartBuyerTorrentPlugin::setInfo_hash(const libtorrent::sha1_hash &info_hash) {
    _infoHash = info_hash;
}

