/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#include "SessionSpy.hpp"

//// ConnectionSpy

template <class ConnectionIdType>
ConnectionSpy<ConnectionIdType>::ConnectionSpy(const ConnectionIdType & id)
    : _id(id) {
}

template <class ConnectionIdType>
protocol_wire::ExtendedMessagePayload * ConnectionSpy<ConnectionIdType>::message() const {
    return _message;
}

template <class ConnectionIdType>
bool ConnectionSpy<ConnectionIdType>::messageSent() const {
    return _messageSent;
}

template <class ConnectionIdType>
void ConnectionSpy<ConnectionIdType>::reset() {
    _messageSent = false;
}

template <class ConnectionIdType>
SendMessageOnConnection ConnectionSpy<ConnectionIdType>::sendHook() {
    return;
}

//// SessionSpy

template <class ConnectionIdType>
SessionSpy<ConnectionIdType>::SessionSpy()
    : _session(nullptr) {
}

template <class ConnectionIdType>
Session<ConnectionIdType> * SessionSpy<ConnectionIdType>::createMonitoredSession() {

    if(_session != nullptr)
        throw std::runtime_error();

    _session = new Session<ConnectionIdType>();
    return _session;
}

template <class ConnectionIdType>
ConnectionSpy<ConnectionIdType> * SessionSpy<ConnectionIdType>::addMonitoredConnection(const ConnectionIdType & id) {

    ConnectionSpy<ConnectionIdType> * spy = new ConnectionSpy<ConnectionIdType>(id);

    _connectionSpies.insert(id, spy);

    return spy;
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::reset() {

    _connectionRemoved = false;
    _keyPairsGenerated = false;
    _numberOfGenerateP2PKHAddresses = false;

    // reset connections
    for(auto itr : _connectionSpies)
        itr.second.reset();
}
