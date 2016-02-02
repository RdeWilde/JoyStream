/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTEAM_EXTENSION_PEERPLUGIN_ID_HPP
#define JOYSTEAM_EXTENSION_PEERPLUGIN_ID_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/socket.hpp>

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
Q_DECLARE_METATYPE(joystream::extension::PeerPluginId)

#endif // JOYSTEAM_EXTENSION_PEERPLUGIN_ID_HPP
