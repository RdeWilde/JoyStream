/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol/Session.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Session<ConnectionIdType>::Session(const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnectionCallbackHandler,
                                       const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                       const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : _core(removedConnectionCallbackHandler,
                generateKeyPairsCallbackHandler,
                generateP2PKHAddressesCallbackHandler) {
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
    void Session<ConnectionIdType>::addConnection(const ConnectionIdType & id) {

        /**
        [this](void) {
            _session->invitedToOutdatedContract(_connectionId);
        },
        [this](const joystream::wire::ContractInvitation & invitation) {
            _session->invitedToJoinContract(_connectionId,invitation);
        },
        [this](const joystream::wire::ExtendedMessagePayload * m) {
            _sendMessageOnConnection(_connectionId, m);
        },
        [this](const Coin::typesafeOutPoint & o) {
            _session->contractPrepared(_connectionId, o);
        },
        [this](int i) {
            _session->pieceRequested(_connectionId, i);
        },
        [this]() {
            _session->invalidPieceRequested(_connectionId);
        },
        [this]() {
            _session->paymentInterrupted(_connectionId);
        },
        [this](const Coin::Signature & s) {
            _session->receivedValidPayment(_connectionId, s);
        },
        [this](const Coin::Signature & s) {
            _session->receivedInvalidPayment(_connectionId, s);
        },
        [this]() {
            _session->sellerHasJoined(_connectionId);
        },
        [this]() {
            _session->sellerHasInterruptedContract(_connectionId);
        },
        [this](const joystream::wire::PieceData & p) {
            _session->receivedFullPiece(_connectionId, p);
        },
        0
        */
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::hasConnection(const ConnectionIdType & id) const {
        return _core.hasConnection(id);
    }

    template<class ConnectionIdType>
    bool Session<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

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

}
}
