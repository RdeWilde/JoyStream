/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 29 2016
 */

#include <protocol_session/Session.hpp>
#include <protocol_session/detail/Observing.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Selling.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Observing<ConnectionIdType>::Observing(Session<ConnectionIdType> * session)
        : _session(session) {

        // Update terms for all connections
        for(auto itr : _session->_connections)
            (itr.second)->processEvent(joystream::protocol_statemachine::event::ObserveModeStarted());
    }

    template <class ConnectionIdType>
    uint Observing<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        // Create connection
        detail::Connection<ConnectionIdType> * connection = _session->createAndAddConnection(id, callback);

        // Choose mode on connection
        connection->processEvent(protocol_statemachine::event::ObserveModeStarted());

        return _session->_connections.size();
    }

    template <class ConnectionIdType>
    void Observing<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        if(_session->_state == SessionState::stopped)
            throw exception::StateIncompatibleOperation("cannot remove connection while session is stopped, all connections are removed.");

        if(!_session->hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        _session->destroyConnection(id);
    }

    template <class ConnectionIdType>
    void Observing<ConnectionIdType>::start() {

        // Update state
        _session->_state = SessionState::started;
    }

    template <class ConnectionIdType>
    void Observing<ConnectionIdType>::stop() {

        // Update state
        _session->_state = SessionState::stopped;
    }

    template <class ConnectionIdType>
    void Observing<ConnectionIdType>::pause() {

        // Update state
        _session->_state = SessionState::paused;
    }

}
}
}
