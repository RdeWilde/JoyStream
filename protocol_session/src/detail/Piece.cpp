/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol_session/detail/Piece.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece()
        : _index(0)
        , _state(State::unassigned)
        , _connectionId(connectionId)
        , _length(0) {
    }

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece(int index, State state, const ConnectionIdType & id, int length)
        : _index(index)
        , _state(state)
        , _connectionId(id)
        , _length(length){
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::assigned(const ConnectionIdType & id) {

        assert(_state == State::unassigned);

        _state = State::assigned_to_peer_for_download;
        _connectionId = id;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::deAssign() {
        assert(_state == State::assigned_to_peer_for_download);
        _state = State::unassigned;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::downloaded() {
        _state = State::downloaded;
    }

    template <class ConnectionIdType>
    int Piece<ConnectionIdType>::index() const {
        return _index;
    }

    template <class ConnectionIdType>
    typename Piece<ConnectionIdType>::State Piece<ConnectionIdType>::state() const {
        return _state;
    }

    template <class ConnectionIdType>
    ConnectionIdType Piece<ConnectionIdType>::connectionId() const {
        return _connectionId;
    }

    template <class ConnectionIdType>
    int Piece<ConnectionIdType>::length() const {
        return _length;
    }
}
}
}
