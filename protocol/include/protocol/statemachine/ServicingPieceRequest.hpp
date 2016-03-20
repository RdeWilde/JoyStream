/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP

#include <protocol/statemachine/Selling.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class PieceRequested;
    class ServicingPieceRequest : public sc::simple_state<ServicingPieceRequest, Selling, PieceRequested> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::Observe>>,
                                sc::custom_reaction<event::Recv<wire::Buy>>,
                                sc::custom_reaction<event::Recv<wire::Sell>>
                                > reactions;

        ServicingPieceRequest();

        // Event handlers
        sc::result react(const event::Recv<wire::Observe> &);
        sc::result react(const event::Recv<wire::Buy> &);
        sc::result react(const event::Recv<wire::Sell> &);
    };
}
}
}

// Required to make ServicingPieceRequest complete when included throught his header file,
// as PieceRequested is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/PieceRequested.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP
