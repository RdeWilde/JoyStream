/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_RECV_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_RECV_HPP

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    // T must have copy constructor
    template <class T>
    class Recv : public sc::event<Recv<T>> {

    public:

        Recv();
        Recv(const T &);

        T message() const;
        void setMessage(const T &);

    private:

        T _message;
    };

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_statemachine/../../src/event/Recv.cpp>

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_RECV_HPP

