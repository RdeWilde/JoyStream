/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_STATUS_HPP
#define JOYSTREAM_EXTENSION_STATUS_HPP

#include <extension/BEPSupportStatus.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/socket.hpp>
#include <libtorrent/sha1_hash.hpp>

#include <map>

namespace joystream {
namespace extension {
namespace status {

    struct PeerPlugin {

        PeerPlugin() {}

        PeerPlugin(const libtorrent::tcp::endpoint & endPoint,
                   const BEPSupportStatus & peerBEP10SupportStatus,
                   const BEPSupportStatus & peerBitSwaprBEPSupportStatus)
            : endPoint(endPoint)
            , peerBEP10SupportStatus(peerBEP10SupportStatus)
            , peerBitSwaprBEPSupportStatus(peerBitSwaprBEPSupportStatus) {
        }

        // Endpoint: can be deduced from connection, but is worth keeping if connection pointer becomes invalid
        libtorrent::tcp::endpoint endPoint;

        // Indicates whether peer supports BEP10
        BEPSupportStatus peerBEP10SupportStatus;

        // Indicates whether peer supports BEP43 .. BitSwapr
        BEPSupportStatus peerBitSwaprBEPSupportStatus;

    };

    struct TorrentPlugin {

        TorrentPlugin() {}

        TorrentPlugin(const libtorrent::sha1_hash & infoHash,
                      const protocol_session::status::Session<libtorrent::tcp::endpoint> & session)
            : infoHash(infoHash)
            , session(session) {
        }

        // Torrent info hash
        libtorrent::sha1_hash infoHash;

        // Status of session
        protocol_session::status::Session<libtorrent::tcp::endpoint> session;
    };

}
}
}

#endif // JOYSTREAM_EXTENSION_STATUS_HPP
