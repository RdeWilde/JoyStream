/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 26 2016
 */

#include <protocol/statemachine/ReadyToInviteSeller.hpp>
#include <protocol/statemachine/exception/CannotInviteNonSeller.hpp>
#include <protocol/statemachine/WaitingForSellerToJoin.hpp>
#include <wire/JoinContract.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    ReadyToInviteSeller::ReadyToInviteSeller() {
        std::cout << "Entering ReadyToInviteSeller state." << std::endl;
    }

    sc::result ReadyToInviteSeller::react(const event::InviteSeller & e) {

        std::cout << "Reacting to InviteSeller." << std::endl;

        // Make sure the peer is actually a buyer, otherwise throw exception
        joystream::protocol::PeerModeAnnounced peerMode = context<CBStateMachine>().peerAnnouncedMode();

        if(peerMode.modeAnnounced() != ModeAnnounced::sell)
            throw exception::CannotInviteNonSeller();

        // Update payor state based on invitation
        context<CBStateMachine>()._payor.setFunds(e.value());
        context<CBStateMachine>()._payor.setPayorContractKeyPair(e.buyerContractKeyPair());
        context<CBStateMachine>()._payor.setPayeeFinalPkHash(e.finalPkHash());

        // Send invitation message to seller
        context<CBStateMachine>()._sendMessage(new joystream::wire::JoinContract(joystream::wire::ContractInvitation(e.value(), e.buyerContractKeyPair().pk(), e.finalPkHash()), peerMode.index()));

        // Start waiting for the seller to join
        return transit<WaitingForSellerToJoin>();
    }

}
}
}
