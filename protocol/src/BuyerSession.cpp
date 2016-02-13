/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/BuyerSession.hpp>

namespace joystream {
namespace protocol {

    BuyerSession::BuyerSession(Coin::Network network,
                               const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                               const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                               const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                               std::map<std::string, BuyerConnection> connections,
                               BuyerSessionState state,
                               const BuyerTerms & terms,
                               const joystream::paymentchannel::Payor & payor,
                               const std::vector<Piece> & pieces)
        : Session(Mode::buy, network, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _connections(connections)
        , _state(state)
        , _terms(terms)
        , _payor(payor)
        , _pieces(pieces) {

        // _numberOfUnassignedPieces

        // _namesOfConnectionsWithoutPieceAssignment

        // _assignmentLowerBound

        throw std::runtime_error("not yet implemented, do not forget this state init.");
    }

    void BuyerSession::addConnection(const Connection & connection) {

    }

    void BuyerSession::removeConnection(const std::string & name) {

    }

    void BuyerSession::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

    }
}
}
