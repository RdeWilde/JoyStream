/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol_session/detail/Piece.hpp>
#include <protocol_session/PieceInformation.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece()
        : _index(0)
        , _state(State::unassigned)
        , _connectionId(connectionId)
        , _size(0) {
    }

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece(int index, State state, const ConnectionIdType & id, unsigned int size)
        : _index(index)
        , _state(state)
        , _connectionId(id)
        , _size(size){
    }

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece(const PieceInformation & p)
        : _index(p.index())
        , _state(p.downloaded() ? detail::Piece<ConnectionIdType>::State::downloaded : detail::Piece<ConnectionIdType>::State::unassigned)
        , _size(p.size()) {
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::assigned(const ConnectionIdType & id) {

        assert(_state == State::unassigned);

        _state = State::being_downloaded;
        _connectionId = id;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::unAssign() {
        assert(_state == State::being_downloaded);
        _state = State::unassigned;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::downloaded() {
        _state = State::downloaded;
        _connectionId = ConnectionIdType();
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
    unsigned int Piece<ConnectionIdType>::size() const {
        return _size;
    }
}
}
}
