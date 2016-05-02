/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SESSION_HPP
#define JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

#include <protocol_session/detail/Connection.hpp>
#include <protocol_session/Callbacks.hpp>
#include <protocol_session/SessionMode.hpp>
#include <protocol_session/SessionState.hpp>

// ConnectionIdType: Type for identifying connections.
// 1) must be possible to use as key in std::map
// 2) must have std::string toString(const & ConnectionIdType);

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
}
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    class Selling;

    template <class ConnectionIdType>
    class Buying;

    template <class ConnectionIdType>
    class Observing;
}

    template <class ConnectionIdType>
    class Session {

    public:

        Session();

        //// Manage mode

        // Can only be called when mode is not set, or is in
        // a set mode different to the mode of the call.

        // Change session to observe mode
        void toObserveMode();

        // Change session to sell mode
        void toSellMode();

        // Change session to buy mode
        void toBuyMode();

        //// Manage state

        // Can only be called when state has been set.

        void start();

        void stop();

        void pause();

        //// Common

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        // Adds connection, and return the current number of connections
        uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

        // Whether there is a connection with given id
        bool hasConnection(const ConnectionIdType &) const;

        // Remove connection if one exists with given id, otherwise returns false.
        // NB: does not result in correspondnig callback ??!?!
        bool removeConnection(const ConnectionIdType &);

        // Get vector of all connection ids
        std::vector<ConnectionIdType> connectionIds() const;

        // Process given message on given connection with given ID
        void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload *);

        //// Buying

        // A valid piece was sent too us on given connection
        void validPieceReceivedOnConnection(const ConnectionIdType &, int index);

        // An invalid piece was sent too us on given connection
        void invalidPieceReceivedOnConnection(const ConnectionIdType &, int index);

        // Piece with given index has been downloaded, but not through
        // a regitered connection. Could be non-joystream peers, or something out of bounds.
        void pieceDownloaded(int);

        //// Selling

        // Data for given piece has been loaded
        void pieceLoaded(const protocol_wire::PieceData &, int);

        //// Getters

        SessionState state() const;

        SessionMode mode() const;

    private:

        friend class detail::Observing<ConnectionIdType>;
        friend class detail::Selling<ConnectionIdType>;
        friend class detail::Buying<ConnectionIdType>;

        //// Handle callbacks from connections

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

        //// Utility routines

        // Creates a connection
        detail::Connection<ConnectionIdType> * createConnection(const ConnectionIdType & id, const SendMessageOnConnection &);

        // not sure, should we return connection pointer, or just id?
        std::vector<detail::Connection<ConnectionIdType> *> connectionsWithPeerInMode(protocol_statemachine::ModeAnnounced m);

        template <typename T>
        std::vector<detail::Connection<ConnectionIdType> *> connectionsInState() const;

        // Returns connection if present, otherwise throws exception
        // ConnectionDoesNotExist<ConnectionIdType>
        detail::Connection<ConnectionIdType> * get(const ConnectionIdType &) const;

        //// Members

        // Session mode
        SessionMode _mode;

        // Current state of session
        SessionState _state;

        // Connections
        std::map<ConnectionIdType, detail::Connection<ConnectionIdType> *> _connections;

        // When session was started
        time_t _started;

        //// Substates

        // Each pointer is != nullptr only when _mode corresponds

        // Observer
        detail::Observing<ConnectionIdType> * _observing;

        // Seller
        detail::Selling<ConnectionIdType> * _selling;

        // Buyer
        detail::Buying<ConnectionIdType> * _buying;
    };

}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Session.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

