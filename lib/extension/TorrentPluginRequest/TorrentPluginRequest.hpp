#ifndef TORRENT_PLUGIN_REQUEST_HPP
#define TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequestType.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash

class TorrentPluginRequest
{
public:

    // info hash of torrent plugin target of request
    libtorrent::sha1_hash _info_hash;

    // Constructor
    TorrentPluginRequest(const libtorrent::sha1_hash & info_hash);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const = 0;
};

#endif // TORRENT_PLUGIN_REQUEST_HPP
