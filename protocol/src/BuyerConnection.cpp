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
                                     const wire::JoiningContract & lastJoiningContractReceived,
                                     const std::queue<uint32_t> & downloadedValidPieces)
        : Connection(connection)
        , _clientState(clientState)
        , _lastJoiningContractReceived(lastJoiningContractReceived)
        , _downloadedValidPieces(downloadedValidPieces) {
    }

    BuyerConnection BuyerConnection::createFreshConnection(const Connection & connection) {

        return BuyerConnection(connection,
                               BuyerClientState::no_joystream_message_sent,
                               wire::JoiningContract(),
                               std::queue<uint32_t>());
    }

    BuyerClientState BuyerConnection::clientState() const {
        return _clientState;
    }

    void BuyerConnection::setClientState(const BuyerClientState & clientState) {
        _clientState = clientState;
    }

    wire::JoiningContract BuyerConnection::lastJoiningContractReceived() const {
        return _lastJoiningContractReceived;
    }

    void BuyerConnection::setLastJoiningContractReceived(const wire::JoiningContract & lastJoiningContractReceived) {
        _lastJoiningContractReceived = lastJoiningContractReceived;
    }

    std::queue<uint32_t> BuyerConnection::downloadedValidPieces() const {
        return _downloadedValidPieces;
    }
}
}
