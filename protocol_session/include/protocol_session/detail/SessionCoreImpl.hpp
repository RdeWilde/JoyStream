/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_DETAIL_SESSIONCOREIMPL_HPP
#define JOYSTREAM_PROTOCOLSESSION_DETAIL_SESSIONCOREIMPL_HPP

#include <protocol_session/detail/Connection.hpp>
#include <protocol_session/Callbacks.hpp>

#include <functional>
#include <vector>

namespace joystream {
namespace protocol_session {
namespace detail {

template <class ConnectionIdType>
struct SessionCoreImpl {

    SessionCoreImpl(RemovedConnectionCallbackHandler<ConnectionIdType> removedConnectionCallbackHandler,
                    GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler,
                    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler);

    // not sure, should we return connection pointer, or just id?
    std::vector<Connection<ConnectionIdType> *> connectionsWithPeerInMode(protocol_statemachine::ModeAnnounced m);

    template <typename T>
    std::vector<Connection<ConnectionIdType> *> connectionsInState() const;

    // Whether connection with given ID exists
    bool hasConnection(const ConnectionIdType &) const;

    // Returns connection if present, otherwise throws exception
    // ConnectionDoesNotExist<ConnectionIdType>
    Connection<ConnectionIdType> * get(const ConnectionIdType &) const;

    // Remove a connection which is known to be present
    void remove(const ConnectionIdType &);

    // Connections
    std::map<ConnectionIdType, Connection<ConnectionIdType> *> _connections;

    // Callback for when connection has been removed from session
    RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnectionCallbackHandler;

    // Callback for when key pairs have to be generated
    GenerateKeyPairsCallbackHandler _generateKeyPairsCallbackHandler;

    // Callback for when addresses have to be generated
    GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddressesCallbackHandler;

    // When session was started
    time_t _started;
};

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/SessionCoreImpl.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_DETAIL_SESSIONCOREIMPL_HPP

