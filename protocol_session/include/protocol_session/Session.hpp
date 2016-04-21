/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SESSION_HPP
#define JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

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

        void toObserve();
        void toSellMode();
        void toBuyMode();

        //// Manage connections

        // Adds connection, and return the current number of connections
        uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

        // Whether there is a connection with given id
        bool hasConnection(const ConnectionIdType &) const;

        // Remove connection if one exists with given id, otherwise returns false.
        // NB:does not result in correspondnig callback ??!?!
        bool removeConnection(const ConnectionIdType &);

        // Either we paid for it, or it just came in.
        //void markPieceAsDownloadedAndValid(int index);

        void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload *);

        //
        SessionMode mode() const;

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

#endif // JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

