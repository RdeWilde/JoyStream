/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/observe/Session.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/Buy.hpp>
#include <protocol/wire/Sell.hpp>
#include <protocol/wire/MessageType.hpp>
#include <protocol/sell/Session.hpp>
#include <protocol/sell/Connection.hpp>
#include <protocol/sell/ClientState.hpp>
#include <protocol/sell/PeerState.hpp>

#include <cassert>

namespace joystream {
namespace protocol {
namespace observe {

    Session::Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, const std::map<std::string, Connection> & connections)
        : joystream::protocol::Session(Mode::observe, removedConnectionCallbackHandler)
        , _connections(connections) {
    }

    joystream::protocol::sell::Session * Session::switchToSellMode(const joystream::protocol::sell::Terms & terms, uint32_t numberOfPiecesInTorrent) {

        // New (seller) connections
        std::map<std::string, joystream::protocol::sell::Connection> connections;

        // For each (observer) connection, send a sell mode message, and create a corresponding (seller) connection,
        // and put it in the (seller) connections mapping.
        for(std::map<std::string, Connection>::const_iterator i = connections.cbegin(); i != connections.cend();i++) {

            // Get (observer) connection
            const Connection & c = (*i);


            /*
             * old connection
             *
            quint32 lockTime = max lock time // we always will get max
            quint64 price // we will always get worsst price

            const Coin::KeyPair & payeeContractKeys,
            const Coin::KeyPair & payeePaymentKeys,
            */

            // Make calls to generate keys

            //


            // Create (seller) connection
            /**
            joystream::protocol::sell::Connection connection(c.peerName(),
                                                             c.lastModeAnnouncedByPeer(),
                                                             c.sendMessageCallbackHandler(),
                                                             joystream::protocol::sell::ClientState::seller_mode_announced,
                                                             joystream::protocol::sell::PeerState(),
                                                             joystream::paymentchannel::Payee::initPayeeWithoutKnownPayor(quint32 lockTime,
                                                                                                                            quint64 price,
                                                                                                                            const Coin::KeyPair & payeeContractKeys,
                                                                                                                            const Coin::KeyPair & payeePaymentKeys),
                                                             std::queue<uint32_t>());

            */

            // Send sell mode message
            joystream::protocol::wire::Sell m(terms);
            connection.sendMessageCallbackHandler()(&m);

            // Store in connection map
            connections[c.peerName()] = connection;
        }

        // Create (seller) session
        joystream::protocol::sell::Session s = new joystream::protocol::sell::Session(_removedConnectionCallbackHandler,
                                                                                      connections,
                                                                                      terms,
                                                                                      numberOfPiecesInTorrent);

        // Return
        return s;
    }

    joystream::protocol::buy::Session * Session::switchToSellMode(const joystream::protocol::buy::Terms & terms) {
        return 0;
    }

    void Session::addConnection(const Connection & connection) {

        // Get peer on connection
        std::string name = connection.peerName();

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i != _connections.end())
            throw std::runtime_error("Connection already exists with for given peer.");

        // Store connection
        _connections[name] = connection;

        // Send observer mode message
        joystream::protocol::wire::Observe m;
        connection.sendMessageCallbackHandler()(&m);
    }

    void Session::removeConnection(const std::string & name) {

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Remove connection
        _connections.erase(i);
    }

    void Session::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

        // Check if we actually have a message
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Get message type, and process accordingly
        wire::MessageType type = message.messageType();

        if(type == wire::MessageType::observe) {

            // Recover observe message
            const wire::Observe & m = dynamic_cast<const wire::Observe &>(message);

            // Process
            (*i).second.process(m);

        } else if(type == wire::MessageType::buy) {

            // Recover buy message
            const wire::Buy & m = dynamic_cast<const wire::Buy &>(message);

            // Process
            (*i).second.process(m);

        } else if(type == wire::MessageType::sell) {

            // Recover sell message
            const wire::Sell & m = dynamic_cast<const wire::Sell &>(message);

            // Process
            (*i).second.process(m);

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
}
}
}
