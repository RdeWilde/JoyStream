/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#ifndef BUYINGNAVIGATOR
#define BUYINGNAVIGATOR

#include <protocol_wire/SellerTerms.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/ContractRSVP.hpp>
#include <protocol_wire/PieceData.hpp>
#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace protocol_statemachine {
    class CBStateMachine;
}
}

using namespace joystream;

class BuyingNavigator {

public:

    struct Fixture {

        // Peer (seller) terms
        protocol_wire::SellerTerms peerTerms;
        uint32_t index;

        // Client (buyer) terms
        protocol_wire::BuyerTerms clientTerms;

        // Inviting seller
        quint64 value;
        Coin::KeyPair buyerContractKeyPair;
        Coin::PubKeyHash finalPkHash;

        // Seller joinining
        protocol_wire::ContractRSVP rsvp;

        // Contract ready
        Coin::typesafeOutPoint anchor;

        // Index of piece client (buyer) requests
        int pieceIndex;

        // Piece data sent back by peer (seller)
        protocol_wire::PieceData pieceData;

    };

    BuyingNavigator(const Fixture &);

    // Fast forward routines
    void toBuyMode(protocol_statemachine::CBStateMachine *);
    void toSellerHasJoined(protocol_statemachine::CBStateMachine *);
    void toProcessingPiece(protocol_statemachine::CBStateMachine *);

    // Validation from the payee side
    bool validatePayment(const Coin::Signature &, int) const;

private:

    // Default value
    Fixture _fixture;
};

#endif // BUYINGNAVIGATOR
