/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_PIECEREQUESTED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_PIECEREQUESTED_HPP

#include <protocol/statemachine/ServicingPieceRequest.hpp>
#include <protocol/statemachine/event/PieceLoaded.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class PieceRequested : public sc::simple_state<PieceRequested, ServicingPieceRequest> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::PieceLoaded>
                                > reactions;

        PieceRequested();

        // Event handlers
        sc::result react(const event::PieceLoaded &);
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_PIECEREQUESTED_HPP
