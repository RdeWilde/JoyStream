/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP

#include <protocol/PeerModeAnnounced.hpp>
#include <protocol/statemachine/event/Recv.hpp>

#include <boost/statechart/state_machine.hpp>

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

            CBStateMachine(const PeerModeAnnounced & peerAnnouncedMode = PeerModeAnnounced());

            // Client calls

            // Message arrival

            // Context actions: should not be called direclty
            void clientToObserveMode();
            void clientToSellMode(const joystream::protocol::SellerTerms & terms);
            //void clientToBuyMode();

            // Getters and setters
            joystream::protocol::PeerModeAnnounced peerAnnouncedMode() const;
            void setPeerAnnouncedMode(const joystream::protocol::PeerModeAnnounced & peerAnnouncedMode);

        private:

            //// Peer
            joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;

            //void peerToObserveMode();
            //void peerToSellMode();
            //void peerToBuyMode();

        };
    }
}
}

// Required to make CBStateMachine complete when included throught his header file,
// as ChooseMode is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ChooseMode.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
