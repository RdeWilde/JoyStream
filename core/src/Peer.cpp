/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 13 2016
 */

#include <core/Peer.hpp>

namespace joystream {
namespace core {

Peer::Peer(const libtorrent::peer_info & peerInformation)
    : _peerInformation(peerInformation) {
}

void Peer::update(const libtorrent::peer_info & peerInformation) {

    if(peerInformation.total_download != _peerInformation.total_download)
        emit total_downloadChanged(peerInformation.total_download);

    if(peerInformation.total_upload != _peerInformation.total_upload)
        emit total_upload(peerInformation.total_upload);

    if(peerInformation.payload_up_speed != _peerInformation.payload_up_speed)
        emit payload_up_speed(peerInformation.payload_up_speed);

    if(peerInformation.payload_down_speed != _peerInformation.payload_down_speed)
        emit payload_down_speed(peerInformation.payload_down_speed);

    _peerInformation = peerInformation;
}

}
}
