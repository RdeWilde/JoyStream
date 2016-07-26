/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#include <core/Session.hpp>

namespace joystream {
namespace core {

Session::Session(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status)
    : _mode(status.mode)
    , _state(status.state)
    , _selling(nullptr)
    , _buying(nullptr) {

    // Create connections
    for(auto m : status.connections)
        addConnection(m.second);

    // Create substate
    switch(_mode) {

        case protocol_session::SessionMode::selling:
            _selling = std::shared_ptr<Selling>(new Selling(status.selling));
            break;

        case protocol_session::SessionMode::buying:
            _buying = std::shared_ptr<Buying>(new Buying(status.buying));
            break;

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::observing: break;
    }

}

protocol_session::SessionMode Session::mode() const noexcept {
    return _mode;
}

protocol_session::SessionState Session::state() const noexcept {
    return _state;
}

std::map<libtorrent::tcp::endpoint, std::shared_ptr<Connection> > Session::connections() const noexcept {
    return _connections;
}

std::shared_ptr<Selling> Session::selling() const noexcept {
    return _selling;
}

std::shared_ptr<Buying> Session::buying() const noexcept {
    return _buying;
}

void Session::addConnection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {

    // Ignore if it has already been added
    if(_connections.count(status.connectionId) > 0)
        return;

    // Create conneciton
    std::shared_ptr<Connection> connection(new Connection(status));

    // Add to map
    _connections.insert(std::make_pair(status.connectionId, connection));

    // announce
    emit connectionAdded(connection);
}

void Session::removeConnection(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _connections.find(endPoint);

    // Ignore if it as already gone
    if(it == _connections.cend())
        return;

    // Remove from map
    _connections.erase(it);

    // announce
    emit connectionRemoved(endPoint);
}

void Session::update(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status) {

    if(_mode != status.mode) {
        _mode = status.mode;
        emit modeChanged(status.mode);
    }

    if(_state != status.state) {
        _state = status.state;
        emit stateChanged(status.state);
    }

    /// Update connections

    // for each connection with a status
    for(auto p: status.connections) {

        auto it = _connections.find(p.first);

        // if connection is present, then update
        if(it != _connections.cend())
            it->second->update(p.second);
        else // otherwise add
            addConnection(p.second);

    }

    // for each exisiting peer
    for(auto p: _connections) {

        // if there is no status for it, then remove
        if(status.connections.count(p.first) == 0)
            removeConnection(p.first);
    }

    /// Transition to/Update Substates

    switch(status.mode) {

        case protocol_session::SessionMode::buying:

            if(_mode == protocol_session::SessionMode::buying)
                _buying->update(status.buying);
            else
                _buying = std::shared_ptr<Buying>(new Buying(status.buying));

            break;

        case protocol_session::SessionMode::selling:

            if(_mode == protocol_session::SessionMode::selling)
                _selling->update(status.selling);
            else
                _selling = std::shared_ptr<Selling>(new Selling(status.selling));

            break;

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::observing: break;
    }
}

}
}
