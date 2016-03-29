/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_PROCESSINGPIECE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_PROCESSINGPIECE_HPP

#include <protocol/statemachine/SellerHasJoined.hpp>
#include <protocol/statemachine/event/SendPayment.hpp>
#include <protocol/statemachine/event/InvalidPieceReceived.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ProcessingPiece : public sc::simple_state<ProcessingPiece, SellerHasJoined> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::SendPayment>,
                                sc::custom_reaction<event::InvalidPieceReceived>
                                > reactions;

        ProcessingPiece();

        // Event handlers
        sc::result react(const event::SendPayment &);
        sc::result react(const event::InvalidPieceReceived &);
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_PROCESSINGPIECE_HPP

