/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#ifndef SELLINGNAVIGATOR
#define SELLINGNAVIGATOR

#include <wire/ContractInvitation.hpp>
#include <wire/SellerTerms.hpp>
#include <wire/BuyerTerms.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
    class CBStateMachine;
}
}
}

using namespace joystream::protocol;

class SellingNavigator {

public:


    struct Fixture {

        // Peer (buyer) terms
        joystream::wire::BuyerTerms peerTerms;

        // Client (seller) terms
        joystream::wire::SellerTerms clientTerms;

        //
        joystream::wire::ContractInvitation invitation;

        //

    };

    SellingNavigator(Fixture fixture);

    void toSellMode(statemachine::CBStateMachine *);
    void fastForwardToReadyForPieceRequest(statemachine::CBStateMachine *);
    void fastForwardToLoadingPiece(statemachine::CBStateMachine *);

private:

    // Default values
    Fixture _fixture;
};

#endif // SELLINGNAVIGATOR
