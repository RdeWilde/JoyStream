/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SENDPAYMENT_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SENDPAYMENT_HPP

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class SendPayment : public sc::event<SendPayment> {
    };
}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SENDPAYMENT_HPP

