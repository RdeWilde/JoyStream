#include "TorrentPluginAlert.hpp"

TorrentPluginAlert::TorrentPluginAlert(const libtorrent::sha1_hash & infoHash)
    : _infoHash(infoHash) {
}

libtorrent::sha1_hash TorrentPluginAlert::getInfoHash() const {
    return _infoHash;
}
