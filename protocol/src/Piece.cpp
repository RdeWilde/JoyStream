/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol/Piece.hpp>

namespace joystream {
namespace protocol {

    Piece::Piece()
        : _index(-1) // safe value
        , _state(State::unassigned) {
    }

    Piece::Piece(int index, State state, const std::string & nameOfConnectionAssignedThisPiece)
        : _index(index)
        //, _length(length)
        //, _numberOfBlocks(numberOfBlocks)
        , _state(state)
        , _nameOfConnectionAssignedThisPiece(nameOfConnectionAssignedThisPiece) {
    }

    int Piece::index() const {
        return _index;
    }

    void Piece::setIndex(int index) {
        _index = index;
    }

    Piece::State Piece::state() const {
        return _state;
    }

    void Piece::setState(State state) {
        _state = state;
    }

    std::string Piece::nameOfConnectionAssignedThisPiece() const {
        return _peerPlugin;
    }

    void Piece::setNameOfConnectionAssignedThisPiece(const std::string & peerPlugin) {
        _peerPlugin = peerPlugin;
    }


}
}
