/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#include <core/Session.hpp>
#include <core/detail/detail.hpp>

#include <memory>

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

    /**
    // Create substate
    switch(_mode) {

        case protocol_session::SessionMode::selling:
            _selling = std::make_unique<Selling>(status.selling);
            break;

        case protocol_session::SessionMode::buying:
            _buying = std::make_unique<Buying>(status.buying);
            break;

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::observing: break;
    }
    */

}

Session::~Session() {

    for(auto it = _connections.begin();it != _connections.end();)
        removeConnection(it++);
}

protocol_session::SessionMode Session::mode() const noexcept {
    return _mode;
}

protocol_session::SessionState Session::state() const noexcept {
    return _state;
}

std::map<libtorrent::tcp::endpoint, Connection*> Session::connections() const noexcept {
    return detail::getRawMap<libtorrent::tcp::endpoint, Connection>(_connections);
}

Selling * Session::selling() const noexcept {
    return _selling.get();
}

Buying * Session::buying() const noexcept {
    return _buying.get();
}

void Session::addConnection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {

    // Ignore if it has already been added
    if(_connections.count(status.connectionId) > 0)
        return;

    // Create conneciton
    Connection * c = new Connection(status);

    // Add to map
    _connections.insert(std::make_pair(status.connectionId, std::unique_ptr<Connection>(c)));

    // announce
    emit connectionAdded(c);
}

void Session::removeConnection(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _connections.find(endPoint);
    assert(it != _connections.end());

    removeConnection(it);
}

void Session::removeConnection(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Connection> >::iterator it) {

    libtorrent::tcp::endpoint endPoint = it->first;

    // Remove from map
    _connections.erase(it);

    // announce
    emit connectionRemoved(endPoint);
}

void Session::update(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status) {

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
    for(auto & p: _connections) {

        // if there is no status for it, then remove
        if(status.connections.count(p.first) == 0)
            removeConnection(p.first);
    }

    /// Update mode and tranistion to substates

    updateSubstate(status);

    // If mode changed
    if(_mode != status.mode) {
        _mode = status.mode;
        emit modeChanged(_mode);
    }
}

void Session::updateSubstate(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status) {

    switch(status.mode) {

        case protocol_session::SessionMode::buying:

            if(_mode == status.mode)
                _buying->update(status.buying);
            else {

                // Drop selling mode if we were in that mode
                if(_mode == protocol_session::SessionMode::selling)
                    _selling.reset();

                // Create new buying sub state
                assert(_buying.get() == nullptr);
                _buying.reset(new Buying(status.buying));
            }

            break;

        case protocol_session::SessionMode::selling:

            if(_mode == status.mode)
                _selling->update(status.selling);
            else {

                // Drop buying mode if we were in that mode
                if(_mode == protocol_session::SessionMode::buying)
                    _buying.reset();

                // Create new selling sub state
                assert(_selling.get() == nullptr);
                _selling.reset(new Selling(status.selling));
            }

            break;

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::observing: break;
    }

}

}
}
