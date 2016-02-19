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

    class ObserverSession : public Session<Connection> {

    public:

        ObserverSession(Coin::Network network,
                        const std::map<std::string, Connection> & connections,
                        const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                        const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler);

        // Add fresh connection with peer where only extended handshake has been sent
        bool addFreshConnection(const Connection & connection);

        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        SellerSession * switchToSellMode(const SellerTerms & terms, uint32_t numberOfPiecesInTorrent);
        BuyerSession * switchToBuyMode(const BuyerTerms & terms, const std::vector<Piece> & pieces);

    private:
    };

}
}

#endif // JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

