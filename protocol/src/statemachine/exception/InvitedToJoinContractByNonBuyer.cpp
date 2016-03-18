/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace exception {

    InvitedToJoinContractByNonBuyer::NonBuyModeAnnounced InvitedToJoinContractByNonBuyer::toNonBuyMode(ModeAnnounced mode) {

        switch(mode) {
            case ModeAnnounced::none: return NonBuyModeAnnounced::None; break;
            case ModeAnnounced::observe: return NonBuyModeAnnounced::Observe; break;
            case ModeAnnounced::sell: return NonBuyModeAnnounced::Sell; break;

            default: // ModeAnnounced::Buy
                throw std::runtime_error("ModeAnnounced cannot be Buy.");
        }
    }

    InvitedToJoinContractByNonBuyer::InvitedToJoinContractByNonBuyer(NonBuyModeAnnounced mode)
        : std::runtime_error("JoinContract message arrived from peer not in buy mode, spesifically: x")
        , _peerMode(mode) {
    }

    InvitedToJoinContractByNonBuyer::InvitedToJoinContractByNonBuyer(ModeAnnounced mode)
        : InvitedToJoinContractByNonBuyer(toNonBuyMode(mode)) {
    }

    InvitedToJoinContractByNonBuyer::NonBuyModeAnnounced InvitedToJoinContractByNonBuyer::peerMode() const {
        return _peerMode;
    }

    void InvitedToJoinContractByNonBuyer::setPeerMode(NonBuyModeAnnounced peerMode) {
        _peerMode = peerMode;
    }
}
}
}
}
