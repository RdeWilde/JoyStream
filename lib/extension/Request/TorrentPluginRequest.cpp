#include "TorrentPluginRequest.hpp"

TorrentPluginRequest::TorrentPluginRequest(const libtorrent::sha1_hash & info_hash)
    : _info_hash(info_hash) {
}
libtorrent::sha1_hash TorrentPluginRequest::info_hash() const
{
    return _info_hash;
}

void TorrentPluginRequest::setInfo_hash(const libtorrent::sha1_hash &info_hash)
{
    _info_hash = info_hash;
}

