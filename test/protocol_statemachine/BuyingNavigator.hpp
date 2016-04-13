/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#ifndef BUYINGNAVIGATOR
#define BUYINGNAVIGATOR

#include <protocol_statemachine/event/Recv.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace protocol_statemachine {
    class CBStateMachine;
}
}

using namespace joystream;
using namespace joystream::protocol_statemachine;

class BuyingNavigator {

public:

    struct Fixture {

        // Peer (seller) terms
        event::Recv<protocol_wire::SellerTerms> _peerToSellMode;

        protocol_wire::SellerTerms peerTerms;
        uint32_t index;

        // Client (buyer) terms
        protocol_wire::BuyerTerms clientTerms;

        // Inviting seller
        quint64 value;
        Coin::KeyPair buyerContractKeyPair;
        Coin::PubKeyHash finalPkHash;

        // Seller joinining
        event::Recv<protocol_wire::JoiningContract> rsvp;

        // Contract ready
        Coin::typesafeOutPoint anchor;

        // Index of piece client (buyer) requests
        int pieceIndex;

        // Piece data sent back by peer (seller)
        protocol_wire::PieceData pieceData;

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
