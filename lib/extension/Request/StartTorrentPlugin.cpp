#include "StartTorrentPlugin.hpp"

StartTorrentPlugin::StartTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration)
    : _infoHash(infoHash)
    , _configuration(configuration) {
}

PluginRequestType StartTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartTorrentPlugin;
}

libtorrent::sha1_hash StartTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

const TorrentPlugin::Configuration * StartTorrentPlugin::configuration() const {
    return _configuration;
}

void StartTorrentPlugin::setConfiguration(const TorrentPlugin::Configuration *configuration) {
    _configuration = configuration;
}
