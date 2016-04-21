/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_DETAIL_SESSIONCOREIMPL_HPP
#define JOYSTREAM_PROTOCOLSESSION_DETAIL_SESSIONCOREIMPL_HPP

#include <protocol_session/SessionMode.hpp>
#include <protocol_session/detail/Connection.hpp>

#include <functional>
#include <vector>

namespace joystream {
namespace protocol_session {

//// Callback types

// Callback for handling the removal of a connection from the session: c++11 alias declaration
template <class ConnectionIdType>
using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &)>;

typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;
typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

// Client requires a message to be sent
typedef std::function<void(const protocol_wire::ExtendedMessagePayload &)> SendMessageOnConnection;

// Callback for handling broadcasting a transaction
//typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

// Callback for generating a key pair
//typedef std::function< generate coin::keypair?

// Callback for generating a receive address
//typedef std::function generate address

namespace detail {

template <class ConnectionIdType>
struct SessionCoreImpl {

    SessionCoreImpl(RemovedConnectionCallbackHandler<ConnectionIdType> removedConnectionCallbackHandler,
                    GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler,
                    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler);

    uint addConnection(Connection<ConnectionIdType> * c);

    bool hasConnection(const ConnectionIdType & id) const;


    // not sure, should we return connection pointer, or just id?
    std::vector<Connection<ConnectionIdType> *> connectionsWithPeerInMode(protocol_statemachine::ModeAnnounced m);

    template <typename T>
    std::vector<Connection<ConnectionIdType> *> connectionsInState() const;

    // Mode of session
    SessionMode _mode;

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

