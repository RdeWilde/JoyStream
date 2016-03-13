/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    template <class T>
    class Recv : public sc::event<Recv<T>> {

    public:

        Recv(T * message);

        // Getters and setters
        T const * message() const;
        void setMessage(T * message);

    private:

        // pointer for now
        T const * _message;

    };

}
}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol/../../src/statemachine/event/Recv.cpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_RECV_HPP

