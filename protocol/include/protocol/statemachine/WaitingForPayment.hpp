/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP

#include <protocol/statemachine/ServicingPieceRequest.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class WaitingForPayment : public sc::simple_state<WaitingForPayment, ServicingPieceRequest> {

    public:


    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGFORPAYMENT_HPP
