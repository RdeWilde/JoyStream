/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */


#include <protocol_session/Session.hpp>
#include <protocol_session/Exceptions.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Selling.hpp>
#include <protocol_session/detail/Observing.hpp>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Session<ConnectionIdType>::Session()
        : _mode(SessionMode::not_set)
        , _state(SessionState::stopped)
        , _selling(nullptr)
        , _buying(nullptr) {

        time(&_started);
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toObserveMode() {

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionAlreadyInThisMode();

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                _observing = _buying->toObserveMode();
                delete _buying;
                _buying = nullptr;
                break;

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                _observing = _selling->toObserveMode();
                delete _selling;
                _selling = nullptr;
                break;

            default:
                assert(false);
        }

        switch(_state) {

            case SessionState::paused: _observing->pause(); break;
            case SessionState::started: _observing->start(); break;
            case SessionState::stopped: _observing->stop(); break;
            default:
            assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toSellMode() {

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                _selling = _observing->toSellMode();
                delete _observing;
                _observing = nullptr;
                break;

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                _selling = _buying->toSellMode();
                delete _buying;
                _buying = nullptr;
                break;

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                throw exception::SessionAlreadyInThisMode();

            default:
                assert(false);
        }

        switch(_state) {

            case SessionState::paused: _selling->pause(); break;
            case SessionState::started: _selling->start(); break;
            case SessionState::stopped: _selling->stop(); break;
            default:
            assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toBuyMode() {

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                _buying = _observing->toBuyMode();
                delete _observing;
                _observing = nullptr;
                break;

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                throw exception::SessionAlreadyInThisMode();

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                _buying = _selling->toBuyMode();
                delete _selling;
                _selling = nullptr;
                break;

            default:
                assert(false);
        }

        switch(_state) {

            case SessionState::paused: _buying->pause(); break;
            case SessionState::started: _buying->start(); break;
            case SessionState::stopped: _buying->stop(); break;
            default:
            assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::start() {

        if(_state == SessionState::started)
            throw exception::StateIncompatibleOperation();

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                _observing->start();
                break;

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                _buying->start();
                break;

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                _selling->start();
                break;

            default:
                assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::stop() {

        if(_state == SessionState::stopped)
            throw exception::StateIncompatibleOperation();

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                _observing->stop();
                break;

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                _buying->stop();
                break;

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                _selling->stop();
                break;

            default:
                assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::pause() {


        if(_state == SessionState::paused)
            throw exception::StateIncompatibleOperation();

        switch(_mode) {

            case SessionMode::not_set:

                assert(_observing == nullptr && _buying == nullptr && _selling == nullptr);
                throw exception::SessionModeNotSetException();

            case SessionMode::observing:

                assert(_observing != nullptr && _buying == nullptr && _selling == nullptr);
                _observing->pause();
                break;

            case SessionMode::buying:

                assert(_observing == nullptr && _buying != nullptr && _selling == nullptr);
                _buying->pause();
                break;

            case SessionMode::selling:

                assert(_observing == nullptr && _buying == nullptr && _selling != nullptr);
                _selling->pause();
                break;

            default:
                assert(false);
        }

    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::tick() {

        switch(_mode) {

            case SessionMode::not_set:

                throw exception::SessionModeNotSetException();
                break;

            case SessionMode::observing:

                assert(_observing != nullptr);
                _observing->tick();
                break;

            case SessionMode::buying:

                assert(_buying != nullptr);
                _buying->tick();
                break;

            case SessionMode::selling:

                assert(_selling != nullptr);
                _selling->tick();
                break;

            default:
                assert(false);
        }
    }

    template <class ConnectionIdType>
    uint Session<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        if(_mode == SessionMode::not_set)
            throw exception::SessionModeNotSetException();

        // Do not accept new connection if session is not set
        if(_state == SessionState::stopped)
            throw exception::StateIncompatibleOperation();

        // Check that connection is new, throw exception if not
        if(hasConnection(id))
            throw exception::ConnectionAlreadyAddedException<ConnectionIdType>(id);

        // Create a new connection
        detail::Connection<ConnectionIdType> * connection = createConnection(id, callback);

        // Add to map
        _connections.insert(id, connection);

        // Call substate handler
        switch(_mode) {

            case SessionMode::not_set:

                assert(false);

            case SessionMode::buying:

                assert(_buying != nullptr);
                connection->_machine.process_event(protocol_statemachine::event::BuyModeStarted(_buying->terms()));
                break;

            case SessionMode::selling:

                assert(_selling != nullptr);
                connection->_machine.process_event(protocol_statemachine::event::BuyModeStarted(_selling->terms()));

            case SessionMode::observing: break;

            default:
            assert(false);
        }

        return _connections.size();
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::hasConnection(const ConnectionIdType & id) const {

        if(_mode == SessionMode::not_set)
            throw exception::SessionModeNotSetException();

        return _connections.find(id) != _connections.cend();
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        // Verify that session mode is set
        if(_mode == SessionMode::not_set)
            throw exception::SessionModeNotSetException();

        // Check that connection is new, throw exception if not
        if(!hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        // Call substate handler
        switch(_mode) {

            case SessionMode::not_set:

                assert(false);

            case SessionMode::buying:

                assert(_buying != nullptr);
                _buying->removeConnection(id);
                break;

            case SessionMode::selling:

                assert(_selling != nullptr);
                _selling->removeConnection(id);
                break;

            case SessionMode::observing: break;

            default:
                assert(false);
        }

        // Remove connection from map
        removeAndDelete(id);

        // Delete connection
        detail::Connection<ConnectionIdType> * connection = get(id);
        delete connection;

        return true;
    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::processMessageOnConnection(const ConnectionIdType & id, const protocol_wire::ExtendedMessagePayload * m) {

        if(_mode == SessionMode::not_set)
            throw exception::SessionModeNotSetException();

        // Check that connection is new, throw exception if not
        if(!hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        switch(_mode) {

            case SessionMode::not_set:

                assert(false);

            case SessionMode::buying:

                assert(_buying != nullptr);
                _buying->processMessageOnConnection(id, m);
                break;

            case SessionMode::selling:

                assert(_selling != nullptr);
                _selling->processMessageOnConnection(id, m);
                break;

            case SessionMode::observing:

                assert(_observing != nullptr);
                _observing->processMessageOnConnection(id, m);
                break;

        default:
            assert(false);
        }

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::validPieceReceivedOnConnection(const ConnectionIdType &, int index) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::invalidPieceReceivedOnConnection(const ConnectionIdType &, int index) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::pieceDownloaded(int) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::pieceLoaded(const protocol_wire::PieceData &, int) {

    }

    template<class ConnectionIdType>
    SessionState Session<ConnectionIdType>::state() const {
        return _state;
    }

    template<class ConnectionIdType>
    SessionMode Session<ConnectionIdType>::mode() const {
        return _mode;
    }


    template<class ConnectionIdType>
    void Session<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {

        assert(hasConnection(id));

        switch(_mode) {
            case SessionMode::buying: _buying->peerAnnouncedModeAndTerms(id, a); break;
            case SessionMode::selling: break;
            case SessionMode::observing: break;
            case SessionMode::not_set: throw exception::SessionModeNotSetException();
        default:
            assert(false);
        }

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::invitedToOutdatedContract(const ConnectionIdType &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::invitedToJoinContract(const ConnectionIdType &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::pieceRequested(const ConnectionIdType & id, int i) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::invalidPieceRequested(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::paymentInterrupted(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::sellerHasJoined(const ConnectionIdType &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType &) {

    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &) {

    }

    template<class ConnectionIdType>
    detail::Connection<ConnectionIdType> * Session<ConnectionIdType>::createConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        detail::Connection<ConnectionIdType> connection = new detail::Connection<ConnectionIdType>(
        id,
        [this, &id](const protocol_statemachine::AnnouncedModeAndTerms & a) {
            this->peerAnnouncedModeAndTerms(id, a);
        },
        [this, &id](void) {
            this->invitedToOutdatedContract(id);
        },
        [this, &id]() {
            this->invitedToJoinContract(id);
        },
        [this, &callback](const protocol_wire::ExtendedMessagePayload & m) {
            callback(m);
        },
        [this, &id](const Coin::typesafeOutPoint & o) {
            this->contractPrepared(id, o);
        },
        [this, &id](int i) {
            this->pieceRequested(id, i);
        },
        [this, &id]() {
            this->invalidPieceRequested(id);
        },
        [this, &id]() {
            this->paymentInterrupted(id);
        },
        [this, &id](const Coin::Signature & s) {
            this->receivedValidPayment(id, s);
        },
        [this, &id](const Coin::Signature & s) {
            this->receivedInvalidPayment(id, s);
        },
        [this, &id]() {
            this->sellerHasJoined(id);
        },
        [this, &id]() {
            this->sellerHasInterruptedContract(id);
        },
        [this, &id](const protocol_wire::PieceData & p) {
            this->receivedFullPiece(id, p);
        },
        0);

        return connection;
    }

    /**
    template<class ConnectionIdType>
    void Session<ConnectionIdType>::updateSubstateSessionState(SessionState state) {

        switch(state) {

            case SessionState::paused: break;
            case SessionState::started: break;
            case SessionState::stopped: break;
            default:
            assert(false);
        }

    }
    */


    template <class ConnectionIdType>
    std::vector<detail::Connection<ConnectionIdType> *> Session<ConnectionIdType>::connectionsWithPeerInMode(protocol_statemachine::ModeAnnounced m) {

        std::vector<detail::Connection<ConnectionIdType> *> matches;

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
    std::vector<detail::Connection<ConnectionIdType> *> Session<ConnectionIdType>::connectionsInState() const {

        std::vector<detail::Connection<ConnectionIdType> *> matches;

        // Add ids of all connections
        for(auto mapping: _connections) {

            const protocol_statemachine::CBStateMachine & machine = mapping.second->machine();

            if(machine. template inState<T>())
                matches.push_back(mapping.second);
        }

        return matches;
    }

    template <class ConnectionIdType>
    std::vector<ConnectionIdType> Session<ConnectionIdType>::ids() const {

        std::vector<ConnectionIdType> ids;

        // Add ids of all connections
        for(auto mapping: _connections)
            ids.push_back(mapping.first);

        return ids;
    }

    template <class ConnectionIdType>
    detail::Connection<ConnectionIdType> * Session<ConnectionIdType>::get(const ConnectionIdType & id) const {

        auto itr = _connections.find(id);

        if(itr == _connections.cend())
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);
        else
            return *itr;
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::removeAndDelete(const ConnectionIdType & id) {

        auto itr = _connections.find(id);

        if(itr == _connections.cend())
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);
        else
            _connections.erase(itr);
    }
}
}

