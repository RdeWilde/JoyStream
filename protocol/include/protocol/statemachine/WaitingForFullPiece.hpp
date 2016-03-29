/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORFULLPIECE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORFULLPIECE_HPP

#include <protocol/statemachine/SellerHasJoined.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <protocol/wire/FullPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class WaitingForFullPiece : public sc::simple_state<WaitingForFullPiece, SellerHasJoined> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::FullPiece>>
                                > reactions;

        WaitingForFullPiece();

        // Event handlers
        sc::result react(const event::Recv<wire::FullPiece> &);
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORFULLPIECE_HPP
