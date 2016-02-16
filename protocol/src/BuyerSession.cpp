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
                               const std::map<std::string, BuyerConnection> & connections,
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

    int BuyerSession::inviteSellers() {

        // depending on seller_selection_policy {greedy : first come first serve, that is everyone above cutoff, as we learn about them. Do best if multiple
        // available from get go now when we switch. every_n_seconds: invite as many as needed, as long as above cutoff, with best priotitized, every N seconds.}
        // question: perhpas this logic need not be here at all?????
        //
        // ... send invitation to join contract to best subset of peer which are in correct mode and lower enough price.

        // IMPORTANT: what if we want to auto invite upon joining? (no, forget about that, adds layer of complexity)..
        // tick() : in extension (we need some way to track who was invited for what slot)
        // - minimal amount of time has passed
        // - if we are not in correct state (i.e. not all have joined yet), then return
        // - anyone we invited more than K seconds ago, but who has not joined, clear that invite for given slot in payor
        // - suppose there are N slots in payor for which there is no pending invite, then invite top N which we have not yet invited, and assign to a slot
        // -

    }

    /**
    quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee) {
        return paychanSettlementFee*numberOfSellers + (maxPrice*numberOfSellers)*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
    }
    */
}
}
