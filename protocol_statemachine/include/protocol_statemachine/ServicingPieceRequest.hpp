/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP

#include <protocol_statemachine/Selling.hpp>

namespace joystream {
namespace protocol_statemachine {

    class LoadingPiece;
    class ServicingPieceRequest : public sc::simple_state<ServicingPieceRequest, Selling, LoadingPiece> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<protocol_wire::Observe>>,
                                sc::custom_reaction<event::Recv<protocol_wire::Buy>>,
                                sc::custom_reaction<event::Recv<protocol_wire::Sell>>
                                > reactions;

        ServicingPieceRequest();

        // Event handlers
        sc::result react(const event::Recv<protocol_wire::Observe> &);
        sc::result react(const event::Recv<protocol_wire::Buy> &);
        sc::result react(const event::Recv<protocol_wire::Sell> &);
    };
}
}

// Required to make ServicingPieceRequest complete when included throught his header file,
// as PieceRequested is initial state and thus part of parent state_machine definition
#include <protocol_statemachine/LoadingPiece.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP
