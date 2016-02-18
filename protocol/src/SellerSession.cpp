/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/SellerSession.hpp>
#include <protocol/BuyerSession.hpp>
#include <protocol/ObserverSession.hpp>
#include <protocol/Utilities.hpp>

namespace joystream {
namespace protocol {

    SellerSession::SellerSession(Coin::Network network,
                                 const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                 const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                 const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                 const std::map<std::string, SellerConnection> & connections,
                                 const SellerTerms & terms,
                                 uint32_t numberOfPiecesInTorrent)
        : Session(Mode::sell, network, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _connections(connections)
        , _terms(terms)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent) {
    }

    SellerSession * SellerSession::createFreshSession(Coin::Network network,
                                                    const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                    const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                    const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                    const SellerTerms & terms,
                                                    uint32_t numberOfPiecesInTorrent) {

        // currently, this routine does very little extra lifting... may changes in the future.
        return new SellerSession(network,
                                 removedConnectionCallbackHandler,
                                 generateKeyPairsCallbackHandler,
                                 generateP2PKHAddressesCallbackHandler,
                                 std::map<std::string, SellerConnection>(),
                                 terms,
                                 numberOfPiecesInTorrent);
    }

    void SellerSession::addConnection(const Connection & connection, const Coin::KeyPair & payeeContractKeys, const Coin::KeyPair & payeePaymentKeys) {

        // Make sure connection is not already in session
        if(_connections.find(connection.peerName()) == _connections.cend())
            throw std::runtime_error("Connection already exists in session");

        // Create a (seller) connection which is fresh
        SellerConnection sellerConnection = SellerConnection::createFreshConnection(connection,
                                                                                    _terms,
                                                                                    payeeContractKeys,
                                                                                    payeePaymentKeys);

        // Send sell mode message
        wire::Sell m(_terms);
        sellerConnection.sendMessageCallbackHandler()(&m);

        // Update state of connection
        sellerConnection.setClientState(SellerClientState::seller_mode_announced);

        // Create (seller) connection, and store in mapping
        _connections[c.peerName()] = sellerConnection;
    }

    void SellerSession::removeConnection(const std::string & name) {

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

    void SellerSession::updateTerms(const SellerTerms & terms) {

        // Update terms which will be used for all new connections
        _terms = terms;

        // Do something with existing connections which are in a suitale state?
        //
    }
}
}
