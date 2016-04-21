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
        , _id(id)
        , _length(0) {
    }

    template <class ConnectionIdType>
    Piece<ConnectionIdType>::Piece(int index, State state, const ConnectionIdType & id, int length)
        : _index(index)
        , _state(state)
        , _id(id)
        , _length(length){
    }

    template <class ConnectionIdType>
    int Piece<ConnectionIdType>::index() const {
        return _index;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::setIndex(int index) {
        _index = index;
    }

    template <class ConnectionIdType>
    typename Piece<ConnectionIdType>::State Piece<ConnectionIdType>::state() const {
        return _state;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::setState(State state) {
        _state = state;
    }

    template <class ConnectionIdType>
    ConnectionIdType Piece<ConnectionIdType>::id() const {
        return _id;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::setId(const ConnectionIdType & id) {
        _id = id;
    }

    template <class ConnectionIdType>
    int Piece<ConnectionIdType>::length() const {
        return _length;
    }

    template <class ConnectionIdType>
    void Piece<ConnectionIdType>::setLength(int length) {
        _length = length;
    }

}
}
}
