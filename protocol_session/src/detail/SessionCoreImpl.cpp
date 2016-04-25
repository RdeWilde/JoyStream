/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 21 2016
 */

#include <protocol_session/detail/SessionCoreImpl.hpp>
#include <protocol_session/Exceptions.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

template <class ConnectionIdType>
SessionCoreImpl<ConnectionIdType>::SessionCoreImpl(RemovedConnectionCallbackHandler<ConnectionIdType> removedConnectionCallbackHandler,
                GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler,
                GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler)
    : _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
    , _generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
    , _generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler) {

    // Note starting time
    time(&_started);
}

template <class ConnectionIdType>
std::vector<Connection<ConnectionIdType> *> SessionCoreImpl<ConnectionIdType>::connectionsWithPeerInMode(protocol_statemachine::ModeAnnounced m) {

    std::vector<Connection<ConnectionIdType> *> matches;

    // Copy connection with peer in given mode into matches
    for(auto mapping: _connections) {

        const protocol_statemachine::CBStateMachine & machine = mapping.second->machine();

        if(machine.announcedModeAndTermsFromPeer().modeAnnounced() == m)
            matches.push_back(mapping.first);
    }

    return matches;
}

template <class ConnectionIdType>
template <typename T>
std::vector<Connection<ConnectionIdType> *> SessionCoreImpl<ConnectionIdType>::connectionsInState() const {

    std::vector<Connection<ConnectionIdType> *> matches;

    // Add ids of all connections
    for(auto mapping: _connections) {

        const protocol_statemachine::CBStateMachine & machine = mapping.second->machine();

        if(machine. template inState<T>())
            matches.push_back(mapping.second);
    }

    return matches;
}

template <class ConnectionIdType>
bool SessionCoreImpl<ConnectionIdType>::hasConnection(const ConnectionIdType & id) const {
    return _connections.find(id) != _connections.cend();
}

template <class ConnectionIdType>
Connection<ConnectionIdType> * SessionCoreImpl<ConnectionIdType>::get(const ConnectionIdType & id) const {

    auto itr = _connections.find(id);

    if(itr == _connections.cend())
        throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);
    else
        return *itr;
}

}
}
}
