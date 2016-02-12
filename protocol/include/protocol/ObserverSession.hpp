/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP
#define JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/Connection.hpp>

#include <map>

namespace joystream {
namespace protocol {

    class SellerSession;
    class SellerTerms;
    class BuyerSession;
    class BuyerTerms;
    class Piece;

    class ObserverSession : public Session {

    public:

        ObserverSession(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                        const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                        const std::map<std::string, Connection> & connections);

        virtual void addConnection(const Connection & connection);
        virtual void removeConnection(const std::string & name);
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        SellerSession * switchToSellMode(const SellerTerms & terms, uint32_t numberOfPiecesInTorrent);
        BuyerSession * switchToBuyMode(const BuyerTerms & terms, const std::vector<Piece> & pieces);

    private:

        // Mapping peer name to corresponding connection with peer
        std::map<std::string, Connection> _connections;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

