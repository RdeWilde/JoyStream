/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#include <core/Session.hpp>
#include <core/Connection.hpp>
#include <core/Selling.hpp>
#include <core/Buying.hpp>
#include <core/detail/detail.hpp>
#include <core/Exception.hpp>

#include <memory>

Q_DECLARE_METATYPE(joystream::protocol_session::SessionMode)
Q_DECLARE_METATYPE(joystream::protocol_session::SessionState)
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace core {

void Session::registerMetaTypes() {

    qRegisterMetaType<protocol_session::SessionMode>();
    qRegisterMetaType<protocol_session::SessionState>();
    qRegisterMetaType<libtorrent::tcp::endpoint>();
    Connection::registerMetaTypes();
    Selling::registerMetaTypes();
    Buying::registerMetaTypes();
}

Session::Session(const protocol_session::SessionMode & mode,
                 const protocol_session::SessionState & state,
                 Selling * selling,
                 Buying * buying)
    : _mode(mode)
    , _state(state)
    , _selling(selling)
    , _buying(buying) {
}

Session * Session::create(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status) {

    Session * session;
    switch(status.mode)
    {
        case protocol_session::SessionMode::buying:
            session = new Session(status.mode, status.state, nullptr, Buying::create(status.buying));
            break;
        case protocol_session::SessionMode::selling:
            session = new Session(status.mode, status.state, Selling::create(status.selling), nullptr);
            break;
        case protocol_session::SessionMode::not_set :
        case protocol_session::SessionMode::observing :
            session = new Session(status.mode, status.state, nullptr, nullptr);
            break;
    }

    return session;
}

Session::~Session() {
}

protocol_session::SessionMode Session::mode() const noexcept {
    return _mode;
}

protocol_session::SessionState Session::state() const noexcept {
    return _state;
}

bool Session::sellingSet() const noexcept {
    return _selling.get() != nullptr;
}

Selling * Session::selling() const {

    if(sellingSet())
        return _selling.get();
    else
        throw exception::HandleNotSet();
}

bool Session::buyingSet() const noexcept {
    return _buying.get() != nullptr;
}

Buying * Session::buying() const {

    if(buyingSet())
        return _buying.get();
    else
        throw exception::HandleNotSet();
}

void Session::update(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status) {

    if(_state != status.state) {
        _state = status.state;
        emit stateChanged(status.state);
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

    // dont do actual upgrade , just assert congruents

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
                _buying.reset(Buying::create(status.buying));
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
                _selling.reset(Selling::create(status.selling));
            }

            break;

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::observing: break;
    }

}

}
}
