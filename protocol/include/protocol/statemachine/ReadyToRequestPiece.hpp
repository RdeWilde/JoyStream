/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_READYTOREQUESTPIECE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_READYTOREQUESTPIECE_HPP

#include <protocol/statemachine/SellerHasJoined.hpp>
#include <protocol/statemachine/event/RequestPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ReadyToRequestPiece : public sc::simple_state<ReadyToRequestPiece, SellerHasJoined> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::RequestPiece>
                                > reactions;

        ReadyToRequestPiece();

        // Event handlers
        sc::result react(const event::RequestPiece &);
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_READYTOREQUESTPIECE_HPP
