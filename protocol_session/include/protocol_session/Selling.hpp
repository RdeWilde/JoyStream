/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLING_HPP
#define JOYSTREAM_PROTOCOL_SELLING_HPP

#include <protocol_session/detail/SessionCoreImpl.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace Coin {
    class typesafeOutPoint;
    class Signature;
}

namespace joystream {
namespace protocol_session {

template <class ConnectionIdType>
class Buying;

template <class ConnectionIdType>
class Selling {

public:

    struct State {

        enum class ClientState {

            // Closes all connections, and rejects all new connections.
            stopped,

            // Going through the motions.
            started,

            // REWWRITE:***
            // Stops building contract, or requesting new pieces
            // if contract has been announced. In the latter scenario
            // the last payment is honored. New connections are
            // accepted, but only mode is announced.
            paused
        };

        enum class StartedState {



            // Inviting sellers
            sending_invitations,

            // Requesting and downloading pieces
            downloading,

            // Have all pieces
            download_completed
        };


        State()
            : _clientState(ClientState::stopped)
            , _startedState(StartedState::sending_invitations){
        }

        ClientState _clientState;

        StartedState _startedState;
    };

    class Policy {

    };

    Selling(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
            const GenerateKeyPairsCallbackHandler &,
            const GenerateP2PKHAddressesCallbackHandler &);

    //// Manage connections

    // Connection with givne id has been added (ex-post)
    uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

    // Connection with given id has been removed (ex-post)
    uint removeConnection(const ConnectionIdType &);

    // Process given message on given connection with given ID
    void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload &);

    //
    void pieceLoaded();

    //// Manage mode

    // Update terms
    void updateTerms(const protocol_wire::SellerTerms &);

    void toObserveMode();

    Buying<ConnectionIdType> * toBuyMode();

    //// Manage state

    // Starts a stopped session by becoming fully operational
    void start();

    // Immediately closes all existing connections
    void stop();

    // Accepts new connections, but only advertises mode.
    // All existing connections are gracefully paused so that all
    // incoming messages can be ignored. In particular

    // * Buying mode: stops creating new contracts, or for started contracts it
    // only honors last pending payment, but issues no new piece requests.

    void pause();

    //// Miscellenous

    // Time out processing hook
    // NB: Later give some indication of how to set timescale for this call
    void tick();

    //// Getters and setters


private:

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

    // Reference to core of session
    detail::SessionCoreImpl<ConnectionIdType> _sessionCore;

    // Controls behaviour of session
    Policy _policy;

    // State
    State _state;

    // Terms for selling
    protocol_wire::SellerTerms _terms;

};
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Selling.cpp>

#endif // JOYSTREAM_PROTOCOL_SELLING_HPP
