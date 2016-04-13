/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_statemachine/event/RequestPiece.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    RequestPiece::RequestPiece() {
    }

    RequestPiece::RequestPiece(int pieceIndex)
        : _pieceIndex(pieceIndex) {
    }

    int RequestPiece::pieceIndex() const {
        return _pieceIndex;
    }
}
}
}
