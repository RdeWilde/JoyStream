/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 26 2016
 */

#include <protocol_statemachine/ReadyToInviteSeller.hpp>
#include <protocol_statemachine/exception/CannotInviteNonSeller.hpp>
#include <protocol_statemachine/WaitingForSellerToJoin.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace protocol_statemachine {

    ReadyToInviteSeller::ReadyToInviteSeller() {
        std::cout << "Entering ReadyToInviteSeller state." << std::endl;
    }

    sc::result ReadyToInviteSeller::react(const event::InviteSeller & e) {

        std::cout << "Reacting to InviteSeller." << std::endl;

        // Make sure the peer is actually a buyer, otherwise throw exception
        protocol_statemachine::AnnouncedModeAndTerms peerMode = context<CBStateMachine>().announcedModeAndTermsFromPeer();

        if(peerMode.modeAnnounced() != ModeAnnounced::sell)
            throw exception::CannotInviteNonSeller();

        // Send invitation message to seller
        context<CBStateMachine>()._sendMessage(protocol_wire::JoinContract(peerMode.index()));

        // Start waiting for the seller to join
        return transit<WaitingForSellerToJoin>();
    }

}
}
