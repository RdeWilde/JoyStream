/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP

#include <boost/statechart/event.hpp>
#include <boost/shared_array.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class PieceLoaded : public sc::event<PieceLoaded> {

    public:

        PieceLoaded(const boost::shared_array<char> &, int);

        // Getters and setters
        boost::shared_array<char> data() const;

        int length() const;

    private:

        // Piece data;
        boost::shared_array<char> _data;

        // Byte length of piece
        int _length;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_PIECELOADED_HPP
