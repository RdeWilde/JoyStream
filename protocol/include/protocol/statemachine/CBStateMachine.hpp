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

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace protocol {

    namespace wire {
        class ExtendedMessagePayload;
    }

    namespace statemachine {

        class ChooseMode;

        class CBStateMachine : public sc::state_machine<CBStateMachine, ChooseMode> {

        public:

            //// Notifications

            // Client was invited to expired contract, as indicated by bad index
            typedef std::function<void()> InvitedToOutdatedContract;

            // Client was invited to join given contract, should terms be included? they are available in _peerAnnounced
            typedef std::function<void(const Coin::typesafeOutPoint & anchor, int64_t funds, const Coin::PublicKey & contractPk)> InvitedToJoinContract;

            // Client requires a message to be sent
            typedef std::function<void(const wire::ExtendedMessagePayload *)> Send;

            // Peer announced that contract is now ready, should contract be be included? it was available
            typedef std::function<void(const Coin::typesafeOutPoint &)> ContractIsReady;

            // Peer requested
            typedef std::function<void(int)> PieceRequested;

            CBStateMachine(const InvitedToOutdatedContract & invitedToOutdatedContract,
                           const InvitedToJoinContract & invitedToJoinContract,
                           const Send & sendMessage,
                           const ContractIsReady & contractIsReady,
                           const PieceRequested & pieceRequested,
                           const PeerModeAnnounced & peerAnnouncedMode = PeerModeAnnounced());

            // Get name of current state: ***Varies from compiler to compiler***
            const char * getInnerStateName() const;

            // Context actions: should not be called direclty
            void clientToObserveMode();
            void clientToSellMode(const joystream::protocol::SellerTerms &);
            void clientToBuyMode(const joystream::protocol::BuyerTerms &);

            void peerToObserveMode();
            void peerToSellMode(const joystream::protocol::SellerTerms &, uint32_t);
            void peerToBuyMode(const joystream::protocol::BuyerTerms &);

            // Getters and setters
            InvitedToOutdatedContract invitedToOutdatedContract() const;

            InvitedToJoinContract invitedToJoinContract() const;

            Send sendMessage() const;

            ContractIsReady contractIsReady() const;

            PieceRequested pieceRequested() const;

            joystream::protocol::PeerModeAnnounced peerAnnouncedMode() const;

        private:

            // Callbacks for classifier routines
            InvitedToOutdatedContract _invitedToOutdatedContract;
            InvitedToJoinContract _invitedToJoinContract;
            Send _sendMessage;
            ContractIsReady _contractIsReady;
            PieceRequested _pieceRequested;

            // Peer mode status
            //*** Just factor out modeannounced and index, dont save actual terms? ***
            joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;
        };
    }
}
}

// Required to make CBStateMachine complete when included throught his header file,
// as ChooseMode is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ChooseMode.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
