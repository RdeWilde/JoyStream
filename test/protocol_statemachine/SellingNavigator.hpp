/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#ifndef SELLINGNAVIGATOR
#define SELLINGNAVIGATOR

#include <protocol_statemachine/protocol_statemachine.hpp>

namespace Coin {
    class PrivateKey;
    class Signature;
}

using namespace joystream;
using namespace joystream::protocol_statemachine;

// Simulates both incoming peer messages
// and client events, when state machine is in Selling state
class SellingNavigator {

public:

    // Set of events arriving when in Selling state
    struct Fixture {

        // Peer (buyer) terms
        event::Recv<protocol_wire::Buy> peerToBuyMode;

        // Client (seller) terms
        event::SellModeStarted sellModeStarted;

        // Peer (buyer) invitation with invalid and valid index
        event::Recv<protocol_wire::JoinContract> invalidJoinContract, validJoinContract;

        // Client (seller) accepts invitation
        event::Joined joinedContract;

        // Peer (buyer) announced
        event::Recv<protocol_wire::Ready> contractReady;

        // Peer (buyer) request for valid piece and invalid piece indexes
        event::Recv<protocol_wire::RequestFullPiece> invalidPieceRequest, validPieceRequest;

        // Client (seller) data sent back to peer
        event::PieceLoaded fullPiece;

        // Peer (buyer) sends bad payment
        event::Recv<protocol_wire::Payment> badPayment;

        // Peer (buyer) sends a valid payment
        event::Recv<protocol_wire::Payment> goodPayment(const Coin::PrivateKey &, int = 1) const;
    };

    SellingNavigator(Fixture fixture);

    void toSellMode(CBStateMachine *);
    void toReadyForPieceRequest(CBStateMachine *);
    void toLoadingPiece(CBStateMachine *);
    void toWaitingForPayment(CBStateMachine *);

private:

    // Default values
    Fixture _fixture;
};

#endif // SELLINGNAVIGATOR
