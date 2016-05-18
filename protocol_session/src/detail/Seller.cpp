/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#include <protocol_session/detail/Seller.hpp>
#include <protocol_session/detail/Connection.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Seller<ConnectionIdType>::Seller()
        : _state(State::waiting_to_be_assigned_piece)
        , _connection(nullptr)
        , _indexOfAssignedPiece(0){
    }

    template <class ConnectionIdType>
    Seller<ConnectionIdType>::Seller(State state, Connection<ConnectionIdType> * connection, uint32_t indexOfAssignedPiece)
        : _state(state)
        , _connection(connection)
        , _indexOfAssignedPiece(indexOfAssignedPiece) {
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::requestPiece(int i) {

        assert(_state == State::waiting_to_be_assigned_piece);

        _state = State::waiting_for_full_piece;
        _indexOfAssignedPiece = i;

        // Note piece assignment time, so we later can detect time outs
        time(&_whenLastPieceAssigned);

        // Send request
        assert(_connection != nullptr);
        _connection->processEvent(protocol_statemachine::event::RequestPiece(i));
    }

    template <class ConnectionIdType>
    bool Seller<ConnectionIdType>::servicingPieceHasTimedOut(double timeOutLimit) const{

        if(_state != State::waiting_for_full_piece)
            return false;

        // Get current time
        time_t now = time(0);

        // Whether time limit was exceeded
        return difftime(now, _whenLastPieceAssigned) > timeOutLimit;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::fullPieceArrived() {

        assert(_state == State::waiting_for_full_piece);
        _state = State::waiting_for_piece_validation_and_storage;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::removed() {
        _state = State::gone;
        _connection = nullptr;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::pieceWasValid() {

        assert(_state == State::waiting_for_piece_validation_and_storage);

        // Update state
        _state = State::waiting_to_be_assigned_piece;

        // Make payment if connection exists
        if(_connection != nullptr)
            _connection->processEvent(protocol_statemachine::event::SendPayment());
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::pieceWasInvalid() {

        assert(_state == State::waiting_for_piece_validation_and_storage);

        // We dont update state here, caller decides: for now we dont need new state
        // for this, as connection is immediately removed.

        // Trigger callback to session and terminate state machine
        if(_connection != nullptr)
            _connection->processEvent(protocol_statemachine::event::InvalidPieceReceived());
    }

    template <class ConnectionIdType>
    bool Seller<ConnectionIdType>::isPossiblyOwedPayment() const {
        return _state == State::waiting_for_piece_validation_and_storage ||
               _state == State::waiting_for_full_piece;
    }

    template <class ConnectionIdType>
    typename Seller<ConnectionIdType>::State Seller<ConnectionIdType>::state() const {
        return _state;
    }

    template <class ConnectionIdType>
    Connection<ConnectionIdType> * Seller<ConnectionIdType>::connection() const {
        return _connection;
    }

    template <class ConnectionIdType>
    int Seller<ConnectionIdType>::indexOfAssignedPiece() const {
        return _indexOfAssignedPiece;
    }

}
}
}
