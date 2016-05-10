/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol_statemachine/event/PieceLoaded.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    PieceLoaded::PieceLoaded() {
    }

    PieceLoaded::PieceLoaded(const joystream::protocol_wire::PieceData & pieceData)
        : _pieceData(pieceData) {
    }

    joystream::protocol_wire::PieceData PieceLoaded::pieceData() const {
        return _pieceData;
    }

}
}
}
