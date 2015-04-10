#include "StartObserverTorrentPlugin.hpp"

StartObserverTorrentPlugin::StartObserverTorrentPlugin() {
}

StartObserverTorrentPlugin::StartObserverTorrentPlugin(const libtorrent::sha1_hash & infoHash)
    : _infoHash(infoHash) {
}

PluginRequestType StartObserverTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartObserverTorrentPlugin;
}

libtorrent::sha1_hash StartObserverTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartObserverTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}
