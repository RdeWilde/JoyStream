/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_PIECELOADED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_PIECELOADED_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class PieceLoaded : public sc::event<PieceLoaded> {

    public:

        PieceLoaded();
        PieceLoaded(const joystream::protocol_wire::PieceData &);

        joystream::protocol_wire::PieceData pieceData() const;

    private:

        // Piece data;
        joystream::protocol_wire::PieceData _pieceData;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_PIECELOADED_HPP
