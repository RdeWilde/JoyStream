/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_TORRENT_PLUGIN_REQUEST_HPP
#define JOYSTREAM_EXTENSION_REQUEST_TORRENT_PLUGIN_REQUEST_HPP

#include <extension/request/TorrentPluginRequestType.hpp>

#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace extension {
namespace request {

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

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_TORRENT_PLUGIN_REQUEST_HPP
