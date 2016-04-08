/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#ifndef BUYINGNAVIGATOR
#define BUYINGNAVIGATOR

#include <wire/SellerTerms.hpp>
#include <wire/BuyerTerms.hpp>
#include <wire/ContractRSVP.hpp>
#include <wire/PieceData.hpp>
#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
    class CBStateMachine;
}
}
}

using namespace joystream::protocol::statemachine;

class BuyingNavigator {

public:

    struct Fixture {

        // Peer (seller) terms
        joystream::wire::SellerTerms peerTerms;
        uint32_t index;

        // Client (buyer) terms
        joystream::wire::BuyerTerms clientTerms;

        // Inviting seller
        quint64 value;
        Coin::KeyPair buyerContractKeyPair;
        Coin::PubKeyHash finalPkHash;

        // Seller joinining
        joystream::wire::ContractRSVP rsvp;

        // Contract ready
        Coin::typesafeOutPoint anchor;

        // Index of piece client (buyer) requests
        int pieceIndex;

        // Piece data sent back by peer (seller)
        joystream::wire::PieceData pieceData;

    };

    BuyingNavigator(const Fixture &);

    // Fast forward routines
    void toBuyMode(CBStateMachine *);
    void toSellerHasJoined(CBStateMachine *);
    void toProcessingPiece(CBStateMachine *);

    // Validation from the payee side
    bool validatePayment(const Coin::Signature &, int) const;

private:

    // Default value
    Fixture _fixture;
};

#endif // BUYINGNAVIGATOR
