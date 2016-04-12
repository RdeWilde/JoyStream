/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#include <protocol_session/Seller.hpp>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Seller<ConnectionIdType>::Seller()
        : _state(State::waiting_to_be_assigned_piece)
        , _indexOfAssignedPiece(0) {
    }

    template <class ConnectionIdType>
    Seller<ConnectionIdType>::Seller(State state, const ConnectionIdType & connectionId, uint32_t indexOfAssignedPiece)
        : _state(state)
        , _connectionId(connectionId)
        , _indexOfAssignedPiece(indexOfAssignedPiece) {
    }

    template <class ConnectionIdType>
    typename Seller<ConnectionIdType>::State Seller<ConnectionIdType>::state() const {
        return _state;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::setState(State state) {
        _state = state;
    }

    template <class ConnectionIdType>
    ConnectionIdType Seller<ConnectionIdType>::connectionId() const {
        return _connectionId;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::setConnectionId(const ConnectionIdType & connectionId) {
        _connectionId = connectionId;
    }

    template <class ConnectionIdType>
    uint32_t Seller<ConnectionIdType>::indexOfAssignedPiece() const {
        return _indexOfAssignedPiece;
    }

    template <class ConnectionIdType>
    void Seller<ConnectionIdType>::setIndexOfAssignedPiece(const uint32_t & indexOfAssignedPiece) {
        _indexOfAssignedPiece = indexOfAssignedPiece;
    }
}
}
