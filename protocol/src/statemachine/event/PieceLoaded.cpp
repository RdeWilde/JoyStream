/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol/statemachine/event/PieceLoaded.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    PieceLoaded::PieceLoaded(const PieceData & pieceData)
        : _pieceData(pieceData) {
    }

    PieceData PieceLoaded::pieceData() const {
        return _pieceData;
    }

}
}
}
}
