/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP

#include <protocol/PieceData.hpp>
#include <boost/statechart/event.hpp>


namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class PieceLoaded : public sc::event<PieceLoaded> {

    public:

        PieceLoaded(const PieceData &);

        PieceData pieceData() const;

    private:

        // Piece data;
        PieceData _pieceData;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP
