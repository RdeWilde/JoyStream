#include "TorrentPluginAlert.hpp"

TorrentPluginAlert::TorrentPluginAlert() {

}

TorrentPluginAlert::TorrentPluginAlert(const libtorrent::sha1_hash & infoHash)
    : _infoHash(infoHash) {
}

libtorrent::sha1_hash TorrentPluginAlert::infoHash() const {
    return _infoHash;
}
