/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/SellerSession.hpp>
#include <protocol/BuyerSession.hpp>
#include <protocol/ObserverSession.hpp>
#include <wire/Sell.hpp>

namespace joystream {
namespace protocol {

    SellerSession::SellerSession(Coin::Network network,
                                 const std::map<std::string, SellerConnection> & connections,
                                 const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                 const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                 const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                 const joystream::wire::SellerTerms & terms,
                                 uint32_t numberOfPiecesInTorrent)
        : Session(Mode::sell, network, connections, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _terms(terms)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent) {
    }

    SellerSession * SellerSession::createFreshSession(Coin::Network network,
                                                      const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                      const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                      const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                      const joystream::wire::SellerTerms & terms,
                                                      uint32_t numberOfPiecesInTorrent) {

        return new SellerSession(network,
                                 std::map<std::string, SellerConnection>(),
                                 removedConnectionCallbackHandler,
                                 generateKeyPairsCallbackHandler,
                                 generateP2PKHAddressesCallbackHandler,
                                 terms,
                                 numberOfPiecesInTorrent);
    }

    bool SellerSession::addFreshConnection(const Connection & connection, const Coin::KeyPair & payeeContractKeys, const Coin::PubKeyHash & payeeFinalPubKeyHash) {

        // Make sure connection is not already in session
        if(hasConnection(connection.peerName()))
            return false;

        // Create a (seller) connection which is fresh
        SellerConnection sellerConnection = SellerConnection::createFreshConnection(connection, _terms, payeeContractKeys, payeeFinalPubKeyHash);

        // Send sell mode message
        joystream::wire::Sell m(_terms, 0);
        sellerConnection.sendMessageCallbackHandler()(&m);

        // Update state of connection
        sellerConnection.setClientState(SellerClientState::seller_mode_announced);

        // Create (seller) connection, and store in mapping
        _connections[sellerConnection.peerName()] = sellerConnection;
    }

    bool SellerSession::removeConnection(const std::string & name) {

        // Make sure connection is in session
        if(!hasConnection(name))
            return false;

        // Other "magic stuff", gracefully remove

        // Remove from map
        // invariant: must be true
        return Session<SellerConnection>::removeConnection(name);
    }

    void SellerSession::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

    }

    void SellerSession::tick() {

    }

    BuyerSession * SellerSession::switchToBuyMode(const BuyerTerms & terms) {
        return 0;
    }

    ObserverSession * SellerSession::switchToObserveMode() {
        return 0;
    }

    void SellerSession::updateTerms(const joystream::wire::SellerTerms & terms) {

        // Update terms which will be used for all new connections
        _terms = terms;

        // Do something with existing connections which are in a suitale state?
        //
    }
}
}
