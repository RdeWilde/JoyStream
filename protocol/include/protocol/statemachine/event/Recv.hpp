/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP

#include <boost/statechart/event.hpp>
#include <protocol/wire/ExtendedMessagePayload.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class Recv : public sc::event<Recv> {

    public:

        Recv(joystream::protocol::wire::ExtendedMessagePayload * message);

        // Getters and setters
        joystream::protocol::wire::ExtendedMessagePayload const * message() const;
        void setMessage(joystream::protocol::wire::ExtendedMessagePayload * message);

    private:

        // pointer for now
        joystream::protocol::wire::ExtendedMessagePayload const * _message;

    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP

