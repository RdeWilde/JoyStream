/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PEER_PLUGIN_REQUEST_HPP
#define PEER_PLUGIN_REQUEST_HPP

#include "PeerPluginRequestType.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/socket.hpp>

class PeerPluginRequest
{
public:

    // Hash of target torrent to which peer corresponds
    libtorrent::sha1_hash _info_hash;

    // Endpoint of peer
    libtorrent::tcp::endpoint _endpoint;

    // Constructor
    PeerPluginRequest(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endpoint);

    // Returns type of peer plugin request
    virtual PeerPluginRequestType getPeerPluginRequestType() const = 0;

};

#endif // PEER_PLUGIN_REQUEST_HPP
