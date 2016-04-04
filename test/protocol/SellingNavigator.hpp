/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#ifndef SELLINGNAVIGATOR
#define SELLINGNAVIGATOR

#include <protocol/ContractInvitation.hpp>
#include <protocol/SellerTerms.hpp>
#include <protocol/BuyerTerms.hpp>

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
        BuyerTerms peerTerms;

        // Client (seller) terms
        SellerTerms clientTerms;

        //
        ContractInvitation invitation;

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
