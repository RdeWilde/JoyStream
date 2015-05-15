#include "TorrentPluginAlert.hpp"

TorrentPluginAlert::TorrentPluginAlert(const libtorrent::sha1_hash & infoHash)
    : _infoHash(infoHash) {
}

TorrentPluginAlert::TorrentPluginAlert(const TorrentPluginAlert & alert)
    : _infoHash(alert.infoHash()) {
}

libtorrent::sha1_hash TorrentPluginAlert::infoHash() const {
    return _infoHash;
}
