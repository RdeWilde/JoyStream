#include "TorrentPluginRequest.hpp"

TorrentPluginRequest::TorrentPluginRequest(const libtorrent::sha1_hash & info_hash)
    : _info_hash(info_hash) {
}
