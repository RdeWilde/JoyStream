/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/ObserverSession.hpp>
#include <wire/Observe.hpp>
#include <wire/Buy.hpp>
#include <wire/Sell.hpp>
#include <wire/MessageType.hpp>
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
                                     const std::map<std::string, Connection> & connections,
                                     const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : Session(Mode::observe, network, connections, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler) {
    }

    ObserverSession * ObserverSession::createFreshSession(Coin::Network network,
                                                          const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                          const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                          const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler) {

        return new ObserverSession(network,
                                   std::map<std::string, Connection>(),
                                   removedConnectionCallbackHandler,
                                   generateKeyPairsCallbackHandler,
                                   generateP2PKHAddressesCallbackHandler);
    }

    bool ObserverSession::addFreshConnection(const Connection & connection) {

        // Make sure connection is new
        if(hasConnection(connection.peerName()))
            return false;

        // Send observer mode message
        joystream::wire::Observe m;
        connection.sendMessageCallbackHandler()(&m);

        // Store mapping
        _connections[connection.peerName()] = connection;

        return true;
    }

    void ObserverSession::processMessageOnConnection(const std::string & name, const joystream::wire::ExtendedMessagePayload & message) {

        // Check if we actually have a message
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Get
        Connection & connection = (*i).second;

        // Get message type, and process accordingly
        joystream::wire::MessageType type = message.messageType();

        if(type == joystream::wire::MessageType::observe) {

            // Recover observe message
            const joystream::wire::Observe & m = dynamic_cast<const joystream::wire::Observe &>(message);

            // Process
            connection.process(m);

        } else if(type == joystream::wire::MessageType::buy) {

            // Recover buy message
            const joystream::wire::Buy & m = dynamic_cast<const joystream::wire::Buy &>(message);

            // Process
            connection.process(m);

        } else if(type == joystream::wire::MessageType::sell) {

            // Recover sell message
            const joystream::wire::Sell & m = dynamic_cast<const joystream::wire::Sell &>(message);

            // Process
            connection.process(m);

        } else if(type == joystream::wire::MessageType::join_contract ||
                  type == joystream::wire::MessageType::joining_contract ||
                  type == joystream::wire::MessageType::sign_refund ||
                  type == joystream::wire::MessageType::refund_signed ||
                  type == joystream::wire::MessageType::ready ||
                  type == joystream::wire::MessageType::request_full_piece ||
                  type == joystream::wire::MessageType::full_piece ||
                  type == joystream::wire::MessageType::payment) {

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

    SellerSession * ObserverSession::switchToSellMode(const joystream::wire::SellerTerms & terms, uint32_t numberOfPiecesInTorrent) const {
        return SellerSession::convertToSellerSession(this, terms, numberOfPiecesInTorrent);
    }

    BuyerSession * ObserverSession::switchToBuyMode(const joystream::wire::BuyerTerms & terms, const Coin::UnspentP2PKHOutput & utxo, const std::vector<Piece> & pieces) const {
        return BuyerSession::convertToBuyerSession(this, terms, utxo, pieces);
    }

}
}
