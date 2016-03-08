/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP

#include <protocol/statemachine/Buy.hpp>
#include <protocol/statemachine/Sell.hpp>
#include <protocol/PeerModeAnnounced.hpp>

namespace joystream {
namespace protocol {

    namespace wire {
        class Sell;
        class Buy;
        class Observe;
    }

namespace statemachine {

    class CBStateMachine {

    public:

        enum class State {

            // simple state:
            observe,

            // complex state:
            sell,

            // complex state:
            buy
        };

        CBStateMachine(State state);

        //// Events


        // Getters
        State state() const;

    private:

        // State which machine is in
        State _state;

        //// Composite states

        // Composite state buy
        Buy _buy;

        // Composite state sell
        Sell _sell;

        //// Peer
        joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
