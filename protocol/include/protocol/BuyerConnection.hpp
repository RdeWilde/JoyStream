/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP

#include <protocol/Connection.hpp>
#include <protocol/BuyerClientState.hpp>
#include <protocol/wire/JoiningContract.hpp>

#include <queue>

namespace joystream {
namespace protocol {

    class BuyerConnection : public Connection {

    public:

        BuyerConnection();

        BuyerConnection(const Connection & connection,
                        BuyerClientState clientState,
                        const wire::JoiningContract & lastJoiningContractReceived,
                        const std::queue<uint32_t> & downloadedValidPieces);

        // Construct connection without any prior state, i.e. has never had any message transmitted/received
        static BuyerConnection createFreshConnection(const Connection & connection);

        // Getters
        BuyerClientState clientState() const;
        void setClientState(const BuyerClientState & clientState);

        wire::JoiningContract lastJoiningContractReceived() const;
        void setLastJoiningContractReceived(const wire::JoiningContract &lastJoiningContractReceived);

        std::queue<uint32_t> downloadedValidPieces() const;

    private:

        // State of client (us) on this connection
        BuyerClientState _clientState;

        // Point in time when last invite sent
        time_t _whenLastInviteSent;

        // Last joining contract message
        wire::JoiningContract _lastJoiningContractReceived;

        // Indexes of valid piecesm, in the order they were downloaded
        // NB: The reason this is not in Seller, is because
        // any peer can potentially provide valid pieces
        std::queue<uint32_t> _downloadedValidPieces;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP
