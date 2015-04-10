#include "StartSellerTorrentPlugin.hpp"

StartSellerTorrentPlugin::StartSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration)
    : _infoHash(infoHash)
    , _configuration(configuration) {
}

// Returns request type
PluginRequestType StartSellerTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartSellerTorrentPlugin;
}

libtorrent::sha1_hash StartSellerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartSellerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

SellerTorrentPlugin::Configuration StartSellerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartSellerTorrentPlugin::setConfiguration(const SellerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}


