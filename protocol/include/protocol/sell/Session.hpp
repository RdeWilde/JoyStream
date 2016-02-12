/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_SELL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_SELL_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/sell/Connection.hpp>
#include <protocol/sell/Terms.hpp>

#include <map>

namespace joystream {
namespace protocol {
namespace buy {
    class Terms;
}

namespace sell {

    class Session : public joystream::protocol::Session {

    public:

        Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                const std::map<std::string, Connection> & connections,
                const Terms & terms,
                uint32_t numberOfPiecesInTorrent);

        // Update terms in the same mode
        void updateTerms(const sell::Terms & terms);

        // Switch from one mode to another
        void switchToBuyMode(const buy::Terms & terms);
        void switchToObserveMode();

        // stop selling (hard, or until last piece)
        // start buying

        // Requested piece read from disk
        // pieceRead(data)

    private:

        // Mapping peer name to corresponding connection with peer
        std::map<std::string, Connection> _connections;

        // Terms for selling, all new connections will have these terms,
        // all existing ones which are beyond mode state will keep their own
        // terms for rest session (until potential reset)
        Terms _terms;

        // The number of pieces in the torrent in question
        uint32_t _numberOfPiecesInTorrent;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_SELL_SESSION_HPP

