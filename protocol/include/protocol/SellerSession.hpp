/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_SELLER_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_SELLER_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/SellerConnection.hpp>
#include <protocol/SellerTerms.hpp>

namespace joystream {
namespace protocol {

    class BuyerTerms;
    class BuyerSession;
    class ObserverSession;
    class Connection;

    class SellerSession : public Session<SellerConnection> {

    public:

        // Construct fully specified session
        SellerSession(Coin::Network network,
                      const std::map<std::string, SellerConnection> & connections,
                      const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                      const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                      const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                      const SellerTerms & terms,
                      uint32_t numberOfPiecesInTorrent);

        // Construct session without any prior state
        static SellerSession * createFreshSession(Coin::Network network,
                                                  const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                  const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                  const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                  const SellerTerms & terms,
                                                  uint32_t numberOfPiecesInTorrent);

        // Construct session based on preexisting session
        template <class T>
        static SellerSession * convertToSellerSession(const Session<T> * session, const SellerTerms & terms, uint32_t numberOfPiecesInTorrent);

        // Add fresh connection with peer where only extended handshake has been sent
        bool addFreshConnection(const Connection & connection, const Coin::KeyPair & payeeContractKeys, const Coin::PubKeyHash & payeeFinalPubKeyHash);

        virtual bool removeConnection(const std::string & name);
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);
        virtual void tick();

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        BuyerSession * switchToBuyMode(const BuyerTerms & terms);
        ObserverSession * switchToObserveMode();

        // Update terms in the same mode
        void updateTerms(const SellerTerms & terms);

        // stop selling (hard, or until last piece)
        // start buying

        // Requested piece read from disk
        // pieceRead(data)

    private:

        // Terms for selling, all new connections will have these terms,
        // all existing ones which are beyond mode state will keep their own
        // terms for rest session (until potential reset)
        SellerTerms _terms;

        // The number of pieces in the torrent in question
        uint32_t _numberOfPiecesInTorrent;
    };

    template <class T>
    SellerSession * SellerSession::convertToSellerSession(const Session<T> * session, const SellerTerms & terms, uint32_t numberOfPiecesInTorrent) {

        // Create (seller) session
        SellerSession * sellerSession = SellerSession::createFreshSession(session->network(),
                                                                          session->removedConnectionCallbackHandler(),
                                                                          session->generateKeyPairsCallbackHandler(),
                                                                          session->generateP2PKHAddressesCallbackHandler(),
                                                                          terms,
                                                                          numberOfPiecesInTorrent);

        // Get connections
        std::map<std::string, T> connections = session->connections();

        // Get callback for generating key pairs
        typename Session<T>::GenerateKeyPairsCallbackHandler handler = session->generateKeyPairsCallbackHandler();

        // Add all connections to session
        for(typename std::map<std::string, T>::const_iterator i = connections.cbegin(); i != connections.cend();i++)
            sellerSession->addFreshConnection((*i).second, handler(1).front(), handler(1).front().pk().toPubKeyHash());

        return sellerSession;
    }
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_SELLER_SESSION_HPP

