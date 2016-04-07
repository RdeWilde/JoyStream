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
#include <wire/PieceData.hpp>
#include <wire/ContractRSVP.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>

namespace Coin {
    class PrivateKey;
    class Signature;
}

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

        // Invitation from peer (buyer)
        joystream::wire::ContractInvitation invitation;

        // Client (seller) contract keys, also used for rsvp
        Coin::KeyPair payeeContractKeyPair;
        Coin::PubKeyHash payeeFinalPkHash;


        // Peer (buyer) contract anchor
        Coin::typesafeOutPoint anchor;

        // Peer (buyer) request for valid piece
        int invalidPieceIndex;
        int validPieceIndex;

        // Client (seller) data sent back to peer
        joystream::wire::PieceData data;
    };

    SellingNavigator(Fixture fixture);

    void toSellMode(statemachine::CBStateMachine *);
    void toReadyForPieceRequest(statemachine::CBStateMachine *);
    void toLoadingPiece(statemachine::CBStateMachine *);
    void toWaitingForPayment(statemachine::CBStateMachine *);

    Coin::Signature payorSignature(const Coin::PrivateKey &, int numberOfPayments = 1) const;

private:

    // Default values
    Fixture _fixture;
};

#endif // SELLINGNAVIGATOR
