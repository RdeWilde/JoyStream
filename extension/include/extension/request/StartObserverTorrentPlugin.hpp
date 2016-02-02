/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_START_OBSERVER_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_REQUEST_START_OBSERVER_TORRENT_PLUGIN_HPP

#include <extension/request/PluginRequest.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace extension {
namespace request {

    class StartObserverTorrentPlugin : public PluginRequest {

    public:

        // Constructor
        StartObserverTorrentPlugin();

        // Constructor from members
        StartObserverTorrentPlugin(const libtorrent::sha1_hash & infoHash);

        // Returns request type
        virtual PluginRequestType getPluginRequestType() const;

        // Getters
        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash & infoHash);

    private:

        // Torrent info_hash
        libtorrent::sha1_hash _infoHash;

        // Observer configuration
    };

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_START_OBSERVER_TORRENT_PLUGIN_HPP
