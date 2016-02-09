/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/observe/Session.hpp>
#include <protocol/wire/Observe.hpp>

namespace joystream {
namespace protocol {
namespace observe {

    Session::Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, const std::map<std::string, Connection> & connections)
        : joystream::protocol::Session(Mode::observe, removedConnectionCallbackHandler)
        , _connections(connections) {
    }

    joystream::protocol::sell::Session * Session::switchToSellMode(const joystream::protocol::sell::Terms & terms) {
        return 0;
    }

    joystream::protocol::buy::Session * Session::switchToSellMode(const joystream::protocol::buy::Terms & terms) {
        return 0;
    }

    void Session::addConnection(const Connection & connection) {

        // Get peer on connection
        std::string name = connection.peerName();

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i != _connections.end())
            throw std::runtime_error("Connection already exists with for given peer.");

        // Store connection
        _connections[name] = connection;

        // Send observer mode message
        joystream::protocol::wire::Observe m;
        connection.sendMessageCallbackHandler()(&m);
    }

    void Session::removeConnection(const std::string & name) {

        // Check if we already have a with this peer
        std::map<std::string, Connection>::iterator i = _connections.find(name);

        if(i == _connections.end())
            throw std::runtime_error("No connection with given peer name exist.");

        // Remove connection
        _connections.erase(i);
    }

    void Session::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload * message) {

    }
}
}
}
