/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/ObserverSession.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/Buy.hpp>
#include <protocol/wire/Sell.hpp>
#include <protocol/wire/MessageType.hpp>
#include <protocol/SellerSession.hpp>
#include <protocol/SellerConnection.hpp>
#include <protocol/SellerClientState.hpp>
#include <protocol/SellerPeerState.hpp>
#include <protocol/BuyerSession.hpp>
#include <protocol/BuyerConnection.hpp>
#include <protocol/BuyerClientState.hpp>
#include <protocol/BuyerPeerState.hpp>
#include <protocol/Piece.hpp>

#include <cassert>

namespace joystream {
namespace protocol {

    ObserverSession::ObserverSession(Coin::Network network,
                                     const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                     const std::map<std::string, Connection> & connections)
        : Session(Mode::observe, network, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _connections(connections) {
    }

    void ObserverSession::addConnection(const Connection & connection) {

        // Get peer on connection
        std::string name = connection.peerName();

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i != _connections.end())
            throw std::runtime_error("Connection already exists with for given peer.");

        // Store connection
        _connections[name] = connection;

        // Send observer mode message
        wire::Observe m;
        connection.sendMessageCallbackHandler()(&m);
    }

    void ObserverSession::removeConnection(const std::string & name) {

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Remove connection
        _connections.erase(i);
    }

    void ObserverSession::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

        // Check if we actually have a message
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Get
        Connection & connection = (*i).second;

        // Get message type, and process accordingly
        wire::MessageType type = message.messageType();

        if(type == wire::MessageType::observe) {

            // Recover observe message
            const wire::Observe & m = dynamic_cast<const wire::Observe &>(message);

            // Process
            connection.process(m);

        } else if(type == wire::MessageType::buy) {

            // Recover buy message
            const wire::Buy & m = dynamic_cast<const wire::Buy &>(message);

            // Process
            connection.process(m);

        } else if(type == wire::MessageType::sell) {

            // Recover sell message
            const wire::Sell & m = dynamic_cast<const wire::Sell &>(message);

            // Process
            connection.process(m);

        } else if(type == wire::MessageType::join_contract ||
                  type == wire::MessageType::joining_contract ||
                  type == wire::MessageType::sign_refund ||
                  type == wire::MessageType::refund_signed ||
                  type == wire::MessageType::ready ||
                  type == wire::MessageType::request_full_piece ||
                  type == wire::MessageType::full_piece ||
                  type == wire::MessageType::payment) {

            // Ignore all other valid messages.
            // The only way thet can arrive from a non-malicious peer is as a result
            // of client switching mode to observer, and
            // the peer sent the given message before it received
            // the new mode message.

        } else {

            // We should never come here, it means we are not covering enum
            assert(false);
        }

    }

    SellerSession * ObserverSession::switchToSellMode(const SellerTerms & terms, uint32_t numberOfPiecesInTorrent) {

        // The number of connections
        uint numberOfConnections = _connections.size();

        // Generate keys and addresses for (seller) connections
        std::vector<Coin::KeyPair> payeeContractKeys = _generateKeyPairsCallbackHandler(numberOfConnections);
        std::vector<Coin::KeyPair> payeePaymentKeys = _generateKeyPairsCallbackHandler(numberOfConnections);

        // New mapping from name to (seller) connections
        std::map<std::string, SellerConnection> connections;

        // For each (observer) connection, send a sell mode message, and create a corresponding (seller) connection,
        // and put it in the (seller) connections mapping.
        uint sellerConnectionIndex = 0;
        for(std::map<std::string, Connection>::const_iterator i = _connections.cbegin(); i != _connections.cend();i++,sellerConnectionIndex++) {

            // Get connection
            const Connection & c = (*i).second;

            // Send sell mode message
            wire::Sell m(terms);
            c.sendMessageCallbackHandler()(&m);

            // Create (seller) connection, and store in mapping
            connections[c.peerName()] = SellerConnection::sellMessageJustSent(c,
                                                                              terms._minLock,
                                                                              terms._minPrice,
                                                                              payeeContractKeys[sellerConnectionIndex],
                                                                              payeePaymentKeys[sellerConnectionIndex]);
        }

        // Create, and return, (seller) session
        return new SellerSession(_network,
                                 _removedConnectionCallbackHandler,
                                 _generateKeyPairsCallbackHandler,
                                 _generateP2PKHAddressesCallbackHandler,
                                 connections,
                                 terms,
                                 numberOfPiecesInTorrent);
    }

    BuyerSession * ObserverSession::switchToBuyMode(const BuyerTerms & terms, const std::vector<Piece> & pieces) {

        // New mapping from name to (seller) connections
        std::map<std::string, BuyerConnection> connections;

        // For each (observer) connection, send a buy mode message, and create a corresponding (buyer) connection,
        // and put it in the (buyer) connections mapping.
        for(std::map<std::string, Connection>::const_iterator i = _connections.cbegin(); i != _connections.cend();i++) {

            // Get connection
            const Connection & c = (*i).second;

            // Send sell mode message
            wire::Buy m(terms);
            c.sendMessageCallbackHandler()(&m);

            // Create (seller) connection, and store in mapping
            connections[c.peerName()] = BuyerConnection::buyMessageJustSent(c);
        }

        // Create payor
        // perha this is done inside a named ctor for BuyerSession???? think about it ??
        joystream::paymentchannel::Payor payor = joystream::paymentchannel::Payor::unknownPayees();

        // complete payor ctr above
        // ... send invitation to join contract to best subset of peer which are in correct mode and lower enough price.

        // Create, and return, (buyer) session
        return new BuyerSession(_network,
                                _removedConnectionCallbackHandler,
                                _generateKeyPairsCallbackHandler,
                                _generateP2PKHAddressesCallbackHandler,
                                connections,
                                BuyerSessionState::waiting_for_full_set_of_sellers,
                                terms,
                                payor,
                                pieces);
    }

}
}
