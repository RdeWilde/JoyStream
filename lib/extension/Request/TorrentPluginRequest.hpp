#ifndef TORRENT_PLUGIN_REQUEST_HPP
#define TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequestType.hpp"

#include <libtorrent/sha1_hash.hpp>

class TorrentPluginRequest {

public:

    // Constructor
    TorrentPluginRequest(const libtorrent::sha1_hash & info_hash);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const = 0;

    // Getters and setters
    libtorrent::sha1_hash info_hash() const;
    void setInfo_hash(const libtorrent::sha1_hash &info_hash);

private:

    // info hash of torrent plugin target of request
    libtorrent::sha1_hash _info_hash;
};

#endif // TORRENT_PLUGIN_REQUEST_HPP
