/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_PEERPLUGIN_ID_HPP
#define EXTENSION_PEERPLUGIN_ID_HPP

//#include <libtorrent/peer.hpp> // sha1_hash
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint

namespace joystream {
namespace extension {

    class PeerPluginId {

    public:

        // Default constructor required for Qt MOC
        PeerPluginId();

        // Constructor from members
        PeerPluginId(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);

        // info hash of torrent to which this peer plugin corresponds
        libtorrent::sha1_hash info_hash_;

        // end point of peer
        libtorrent::tcp::endpoint endPoint_;
    };

}
}

#include <QMetaType>
Q_DECLARE_METATYPE(PeerPluginId)

#endif // EXTENSION_PEERPLUGIN_ID_HPP
