/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#ifndef BUYINGNAVIGATOR
#define BUYINGNAVIGATOR

#include <protocol_statemachine/protocol_statemachine.hpp>

using namespace joystream;
using namespace joystream::protocol_statemachine;

// Simulates both incoming peer messages
// and client events, when state machine is in Buying state
class BuyingNavigator {

public:

    // Set of events arriving when in Buying state
    struct Fixture {

        // Peer (seller) terms
        event::Recv<protocol_wire::Sell> peerToSellMode;

        // Client (buyer) terms
        event::BuyModeStarted buyModeStarted;

        // Client (buyer) invitation
        event::InviteSeller inviteSeller;

        // Peer (seller) joins
        event::Recv<protocol_wire::JoiningContract> joiningContract;

        // Client (buyer) announces contract
        event::ContractPrepared contractPrepared;

        // Client (buyer) requests piece
        event::RequestPiece requestPiece;

        // Peer (seller)
        event::Recv<protocol_wire::FullPiece> fullPiece;

        // Validate client (buyer) payment based on this fixture
        bool validatePayment(const Coin::Signature &, int) const;
    };

    BuyingNavigator(const Fixture &);

    // Fast forward routines
    void toBuyMode(CBStateMachine *);
    void toSellerHasJoined(CBStateMachine *);
    void toProcessingPiece(CBStateMachine *);

private:

    // Default value
    Fixture _fixture;
};

#endif // BUYINGNAVIGATOR
