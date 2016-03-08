/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/Sell.hpp>
#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/wire/JoinContract.hpp>
#include <protocol/wire/Ready.hpp>
#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    Sell::Sell(CBStateMachine * context)
        : _context(context)
        , _state(State::invited)
        , _servicingPieceRequest(this) {
    }

    void Sell::recv(const wire::JoinContract & m) {

    }

    void Sell::recv(const wire::Ready & m) {

    }

    void Sell::recv(const wire::RequestFullPiece & m) {

    }

    void Sell::clientToObserveMode() {

    }

    void Sell::clientToSellMode(const SellerTerms & terms, uint32_t index = 0) {

    }

    void Sell::clientToBuyMode(const BuyerTerms & terms) {

    }

    void Sell::updateSellTerms(const SellerTerms & terms) {

    }

    void Sell::joinContract(const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk) {

    }

    Sell::State Sell::state() const {
        return _state;
    }

}

uint32_t Sell::index() const
{
    return _index;
}

void Sell::setIndex(const uint32_t &index)
{
    _index = index;
}

}
}
