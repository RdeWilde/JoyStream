/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP

#include <protocol/statemachine/ServicingPieceRequest.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <protocol/wire/Payment.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class WaitingForPayment : public sc::simple_state<WaitingForPayment, ServicingPieceRequest> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::Payment>>
                                > reactions;

        WaitingForPayment();

        // Event handlers
        sc::result react(const event::Recv<wire::Payment> &);
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP
