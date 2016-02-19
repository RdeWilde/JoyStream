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

namespace Coin {
    class UnspentP2PKHOutput;
}

namespace joystream {
namespace protocol {

    class SellerSession;
    class SellerTerms;
    class BuyerSession;
    class BuyerTerms;
    class Piece;

    class ObserverSession : public Session<Connection> {

    public:

        // Construct fully specified session
        ObserverSession(Coin::Network network,
                        const std::map<std::string, Connection> & connections,
                        const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                        const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler);

        // Construct session without any prior state
        static ObserverSession * createFreshSession(Coin::Network network,
                                                    const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                    const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                    const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler);

        // Construct session based on preexisting session
        template <class T>
        static ObserverSession * convertToObserverSession(const Session<T> * session);

        // Add fresh connection with peer where only extended handshake has been sent
        bool addFreshConnection(const Connection & connection);

        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        SellerSession * switchToSellMode(const SellerTerms & terms, uint32_t numberOfPiecesInTorrent) const;
        BuyerSession * switchToBuyMode(const BuyerTerms & terms, const Coin::UnspentP2PKHOutput & utxo, const std::vector<Piece> & pieces) const;

    private:
    };

    // Construct session based on preexisting session
    template <class T>
    ObserverSession * ObserverSession::convertToObserverSession(const Session<T> * session) {

        // Create (observer) session
        ObserverSession * observerSession = ObserverSession::createFreshSession(session->network(),
                                                                                session->removedConnectionCallbackHandler(),
                                                                                session->generateKeyPairsCallbackHandler(),
                                                                                session->generateP2PKHAddressesCallbackHandler());

        // Get connections
        std::map<std::string, T> connections = session->connections();

        // Add all connections to session
        for(typename std::map<std::string, T>::const_iterator i = connections.cbegin(); i != connections.cend();i++)
            observerSession->addFreshConnection((*i).second);

        return observerSession;

    }

}
}

#endif // JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

