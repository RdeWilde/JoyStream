/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol_session/Session.hpp>
#include <protocol_session/exception/ConnectionAlreadyAddedException.hpp>
#include <protocol_session/exception/SessionNotSetException.hpp>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Session<ConnectionIdType>::Session(const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnectionCallbackHandler,
                                       const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                       const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : _core(removedConnectionCallbackHandler,
                generateKeyPairsCallbackHandler,
                generateP2PKHAddressesCallbackHandler) {
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::tick() {

        switch(_core._mode) {

            case SessionMode::NotSet: throw exception::SessionNotSetException(); break;
            case SessionMode::Observe: break;
            case SessionMode::Buy: _buying.tick(); break;
            case SessionMode::Sell: _selling.tick(); break;
            default:
                assert(false);
        }
    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toObserve() {

        /**
         * if(_mode == SessionMode::Observe)
            throw std::runtime_error("Already in observe mode.");
        else if(_mode == SessionMode::NotSet);
        */

    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toSellMode() {

    }

    template <class ConnectionIdType>
    void Session<ConnectionIdType>::toBuyMode() {

    }

    template <class ConnectionIdType>
    uint Session<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        // Check that session is set, throw exception if not
        if(_core._mode == SessionMode::NotSet)
            throw exception::SessionNotSetException();

        // Check that connection is new, throw exception if not
        if(_core.hasConnection(id))
            throw exception::ConnectionAlreadyAddedException<ConnectionIdType>(id);

        // Create a new connection
        detail::Connection<ConnectionIdType> connection = new detail::Connection<ConnectionIdType>(
        id,
        [this, &id](const protocol_statemachine::AnnouncedModeAndTerms & a) {
            this->peerAnnouncedModeAndTerms(id, a);
        },
        [this, &id](void) {
            _selling.invitedToOutdatedContract(id);
        },
        [this, &id](const protocol_wire::ContractInvitation & invitation) {
            _selling.invitedToJoinContract(id, invitation);
        },
        [this, &callback](const protocol_wire::ExtendedMessagePayload * m) {
            callback(m);
        },
        [this, &id](const Coin::typesafeOutPoint & o) {
            _selling.contractPrepared(id, o);
        },
        [this, &id](int i) {
            _selling.pieceRequested(id, i);
        },
        [this, &id]() {
            _selling.invalidPieceRequested(id);
        },
        [this, &id]() {
            _selling.paymentInterrupted(id);
        },
        [this, &id](const Coin::Signature & s) {
            _selling.receivedValidPayment(id, s);
        },
        [this, &id](const Coin::Signature & s) {
            _selling.receivedInvalidPayment(id, s);
        },
        [this, &id]() {
            _buying.sellerHasJoined(id);
        },
        [this, &id]() {
            _buying.sellerHasInterruptedContract(id);
        },
        [this, &id](const protocol_wire::PieceData & p) {
            _buying.receivedFullPiece(id, p);
        },
        0);

        // Add to map of connections, get number of connections in session after this
        uint size = _core.addConnection(connection);

        // Notify relevant module
        switch(_core._mode) {
            case SessionMode::Observe: break;
            case SessionMode::Buy: _buying.connectionAdded(id); break;
            case SessionMode::Sell: _selling.connectionAdded(id); break;

            default: // SessionMode::NotSet
            assert(false);
        }

        return size;
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::hasConnection(const ConnectionIdType & id) const {

        // Check that session is set, throw exception if not
        if(_core._mode == SessionMode::NotSet)
            throw exception::SessionNotSetException();

        return _core.hasConnection(id);
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        // Check that session is set, throw exception if not
        if(_core._mode == SessionMode::NotSet)
            throw exception::SessionNotSetException();

        /**
        // Number of connections prior to erase
        typename std::map<std::string, Connection<ConnectionIdType>>::size_type size = _connections.size();

        // Removal was successfull iff size decreased
        return size < _connections.erase(id);
        */

        return true;
    }

    template<class ConnectionIdType>
    SessionMode Session<ConnectionIdType>::mode() const {
        return _core._mode;
    }

    template<class ConnectionIdType>
    void Session<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {

        assert(_core.hasConnection(id));

        switch(_core._mode) {
            case SessionMode::Buy: _buying.peerAnnouncedModeAndTerms(id, a); break;
            case SessionMode::Sell: break;
            case SessionMode::Observe: break;
            case SessionMode::NotSet: throw exception::SessionNotSetException();
        default:
            assert(false);
        }

    }

}
}
