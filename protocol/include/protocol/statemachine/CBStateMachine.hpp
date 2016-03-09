/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP

#include <protocol/PeerModeAnnounced.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {

    namespace wire {
        class Sell;
        class Buy;
        class Observe;
    }

    namespace statemachine {

        class ChooseMode;

        class CBStateMachine : public sc::state_machine<CBStateMachine, ChooseMode> {

        public:

        private:

            //// Peer
            joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;
        };

        class ChooseMode : public sc::simple_state<ChooseMode, CBStateMachine> {

        };

    }
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
