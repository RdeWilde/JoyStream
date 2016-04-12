/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_WAITINGFORPAYMENT_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_WAITINGFORPAYMENT_HPP

#include <protocol_statemachine/ServicingPieceRequest.hpp>
#include <protocol_statemachine/event/Recv.hpp>
#include <protocol_wire/Payment.hpp>

namespace joystream {
namespace protocol_statemachine {

    class WaitingForPayment : public sc::simple_state<WaitingForPayment, ServicingPieceRequest> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<protocol_wire::Payment>>
                                > reactions;

        WaitingForPayment();

        // Event handlers
        sc::result react(const event::Recv<protocol_wire::Payment> &);
    };
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_WAITINGFORPAYMENT_HPP
