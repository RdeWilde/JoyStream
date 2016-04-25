/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SESSION_HPP
#define JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

/**

#include <protocol_session/detail/SessionCoreImpl.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Selling.hpp>

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
}
namespace protocol_session {

    // ConnectionIdType:
    // Type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const

    template <class ConnectionIdType>
    class Session {

    public:


        // Construct session
        Session(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
                const GenerateKeyPairsCallbackHandler &,
                const GenerateP2PKHAddressesCallbackHandler &);

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        //// Manage mode

        // what happens in these depending on what state we are in? (paused, stopped,etc)

        void toObserveMode();

        void toSellMode();

        void toBuyMode();

        //// Manage state

        // Starts a stopped session by becoming fully operational
        void start();

        // Immediately closes all existing connections
        void stop();

        // Accepts new connections, but only advertises mode.
        // All existing connections are gracefully paused so that all
        // incoming messages can be ignored. In particular
        // * Selling mode: stops joining new contracts, and ignores new piece requests.
        // * Buying mode: stops creating new contracts, or for started contracts it
        // only honors last pending payment, but issues no new piece requests.
        void pause();

        //
        void unPause();

        //// Manage connections

        // Adds connection, and return the current number of connections
        uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

        // Whether there is a connection with given id
        bool hasConnection(const ConnectionIdType &) const;

        // Remove connection if one exists with given id, otherwise returns false.
        // NB:does not result in correspondnig callback ??!?!
        bool removeConnection(const ConnectionIdType &);

        // Process given message on given connection with given ID
        void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload *);

        // A valid piece was sent too us on given connection
        void validPieceReceivedOnConnection(const ConnectionIdType &, int index);

        // An invalid piece was sent too us on given connection
        // NB: Perhaps we should supply connection?
        void invalidPieceReceivedOnConnection(const ConnectionIdType &, int index);

        // Piece with given index has been downloaded, but not through
        // a regitered connection. Could be non-joystream peers, or something out of bounds.
        void pieceDownloaded(int);

        //// Getters

        SessionMode mode() const;

        //status::Session<ConnectionIdType> status() const;

    private:

        // Session core
        detail::SessionCoreImpl<ConnectionIdType> _core;

        // Hook for peer mode announcement event on connection with given id
        void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);

        // Observer

        // Seller
        detail::Selling<ConnectionIdType> _selling;

        // Buyer
        detail::Buying<ConnectionIdType> _buying;
    };

}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Session.cpp>

*/

#endif // JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

