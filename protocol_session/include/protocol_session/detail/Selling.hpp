/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLING_HPP
#define JOYSTREAM_PROTOCOL_SELLING_HPP

#include <protocol_session/Session.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace Coin {
    class typesafeOutPoint;
    class Signature;
}

namespace joystream {
namespace protocol_session {
namespace detail {

template <class ConnectionIdType>
class Buying;

template <class ConnectionIdType>
class Observing;

template <class ConnectionIdType>
class Selling {

public:

    Selling(Session<ConnectionIdType> *,
            const RemovedConnectionCallbackHandler<ConnectionIdType> &,
            const GenerateKeyPairsCallbackHandler &,
            const GenerateP2PKHAddressesCallbackHandler &,
            const SellingPolicy &,
            const protocol_wire::SellerTerms &);

    //// Manage connections

    // Connection with given id has been removed (ex-post)
    uint removeConnection(const ConnectionIdType &);

    // Process given message on given connection with given ID
    void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload &);

    // Data for given piece has been loaded
    void pieceLoaded(const protocol_wire::PieceData &, int);

    //// Manage mode

    // Update terms
    void updateTerms(const protocol_wire::SellerTerms &);

    Observing<ConnectionIdType> * toObserveMode();

    Buying<ConnectionIdType> * toBuyMode();

    //// Manage state

    // Starts a stopped session by becoming fully operational
    void start();

    // Immediately closes all existing connections
    void stop();

    // Accepts new connections, but only advertises mode.
    void pause();

    //// Miscellenous

    // Time out processing hook
    // NB: Later give some indication of how to set timescale for this call
    void tick();

    protocol_wire::SellerTerms terms() const;

private:

    // State of context session
    SessionState sessionState() const;

    //// Handling callbacks from connections

    void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
    void invitedToOutdatedContract(const ConnectionIdType &);
    void invitedToJoinContract(const ConnectionIdType &);
    void contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &);
    void pieceRequested(const ConnectionIdType & id, int i);
    void invalidPieceRequested(const ConnectionIdType & id);
    void paymentInterrupted(const ConnectionIdType & id);
    void receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &);
    void receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &);
    void sellerHasJoined(const ConnectionIdType &);
    void sellerHasInterruptedContract(const ConnectionIdType &);
    void receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &);

    //// Members

    // Reference to core of session
    Session<ConnectionIdType> * _session;

    // Callback handlers
    RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnection;
    GenerateKeyPairsCallbackHandler _generateKeyPairs;
    GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddresses;

    // Controls behaviour of session
    SellingPolicy _policy;

    // Terms for selling
    protocol_wire::SellerTerms _terms;

};

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Selling.cpp>

#endif // JOYSTREAM_PROTOCOL_SELLING_HPP
