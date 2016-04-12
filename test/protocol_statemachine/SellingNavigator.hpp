/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#ifndef SELLINGNAVIGATOR
#define SELLINGNAVIGATOR

#include <protocol_wire/ContractInvitation.hpp>
#include <protocol_wire/SellerTerms.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/PieceData.hpp>
#include <protocol_wire/ContractRSVP.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>

namespace Coin {
    class PrivateKey;
    class Signature;
}

namespace joystream {
namespace protocol_statemachine {
    class CBStateMachine;
}
}

using namespace joystream;

class SellingNavigator {

public:

    struct Fixture {

        // Peer (buyer) terms
        protocol_wire::BuyerTerms peerTerms;

        // Client (seller) terms
        protocol_wire::SellerTerms clientTerms;

        // Invitation from peer (buyer)
        protocol_wire::ContractInvitation invitation;

        // Client (seller) contract keys, also used for rsvp
        Coin::KeyPair payeeContractKeyPair;
        Coin::PubKeyHash payeeFinalPkHash;


        // Peer (buyer) contract anchor
        Coin::typesafeOutPoint anchor;

        // Peer (buyer) request for valid piece
        int invalidPieceIndex;
        int validPieceIndex;

        // Client (seller) data sent back to peer
        protocol_wire::PieceData data;
    };

    SellingNavigator(Fixture fixture);

    void toSellMode(protocol_statemachine::CBStateMachine *);
    void toReadyForPieceRequest(protocol_statemachine::CBStateMachine *);
    void toLoadingPiece(protocol_statemachine::CBStateMachine *);
    void toWaitingForPayment(protocol_statemachine::CBStateMachine *);

    Coin::Signature payorSignature(const Coin::PrivateKey &, int numberOfPayments = 1) const;

private:

    // Default values
    Fixture _fixture;
};

#endif // SELLINGNAVIGATOR
