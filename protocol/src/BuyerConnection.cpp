/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/BuyerConnection.hpp>

namespace joystream {
namespace protocol {

    BuyerConnection::BuyerConnection() {
    }

    BuyerConnection::BuyerConnection(const Connection & connection,
                                     BuyerClientState clientState,
                                     BuyerPeerState peerState,
                                     const std::queue<uint32_t> & downloadedValidPieces)
        : Connection(connection)
        , _clientState(clientState)
        , _peerState(peerState)
        , _downloadedValidPieces(downloadedValidPieces) {
    }

    BuyerConnection BuyerConnection::buyMessageJustSent(const Connection & connection) {

        return BuyerConnection(connection,
                               BuyerClientState::buyer_mode_announced,
                               BuyerPeerState(),
                               std::queue<uint32_t>());
    }

    BuyerClientState BuyerConnection::clientState() const {
        return _clientState;
    }

    BuyerPeerState BuyerConnection::peerState() const {
        return _peerState;
    }

    std::queue<uint32_t> BuyerConnection::downloadedValidPieces() const {
        return _downloadedValidPieces;
    }

}
}
