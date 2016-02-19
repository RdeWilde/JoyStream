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
#include <protocol/BuyerSession.hpp>
#include <protocol/BuyerConnection.hpp>
#include <protocol/BuyerClientState.hpp>
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

    void ObserverSession::tick() {

    }

    SellerSession * ObserverSession::switchToSellMode(const SellerTerms & terms, uint32_t numberOfPiecesInTorrent) {

        // Create (seller) session
        SellerSession * session = SellerSession::createFreshSession(_network,
                                                                    _removedConnectionCallbackHandler,
                                                                    _generateKeyPairsCallbackHandler,
                                                                    _generateP2PKHAddressesCallbackHandler,
                                                                    terms,
                                                                    numberOfPiecesInTorrent);

        // Add all (observer) connections to session
        for(std::map<std::string, Connection>::const_iterator i = _connections.cbegin(); i != _connections.cend();i++)
            session->addConnection(*i, terms, _generateKeyPairsCallbackHandler(1), _generateKeyPairsCallbackHandler(1));

        return session;
    }

    BuyerSession * ObserverSession::switchToBuyMode(const BuyerTerms & terms, const Coin::UnspentP2PKHOutput & utxo, const std::vector<Piece> & pieces) {

        // Create (buyer) session
        BuyerSession * session = BuyerSession::createFreshSession(_network,
                                                                  _removedConnectionCallbackHandler,
                                                                  _generateKeyPairsCallbackHandler,
                                                                  _generateP2PKHAddressesCallbackHandler,
                                                                  terms,
                                                                  _generateP2PKHAddressesCallbackHandler(1),
                                                                  utxo,
                                                                  pieces);

        // Add all (observer) connections to session
        for(std::map<std::string, Connection>::const_iterator i = _connections.cbegin(); i != _connections.cend();i++)
            session->addConnection(*i);

        return session;
    }

}
}
