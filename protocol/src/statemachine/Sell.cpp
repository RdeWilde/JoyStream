/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/Sell.hpp>

//#include <protocol/statemachine/CBStateMachine.hpp>
//#include <protocol/wire/JoinContract.hpp>
//#include <protocol/wire/Ready.hpp>
//#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

/**
    Sell::Sell(CBStateMachine * context,
               const InvitedToOutdatedContract & invitedToOutdatedContract,
               const InvitedToJoinContract & invitedToJoinContract,
               const SendJoiningContract & sendJoiningContract,
               const ContractIsReady & contractIsReady,
               const PieceRequested & pieceRequested)
        : _context(context)
        , _state(State::invited)
        //, _servicingPieceRequest(this)
        , _invitedToOutdatedContract(invitedToOutdatedContract)
        , _invitedToJoinContract(invitedToJoinContract)
        , _sendJoiningContract(sendJoiningContract)
        , _contractIsReady(contractIsReady)
        , _pieceRequested(pieceRequested) {
    }

    void Sell::recv(const wire::Observe & m) {

    }

    void Sell::recv(const wire::Buy & m) {

    }

    void Sell::recv(const wire::Sell & m) {

    }

    void Sell::recv(const wire::JoinContract & m) {

    }

    void Sell::recv(const wire::Ready & m) {

    }

    void Sell::recv(const wire::RequestFullPiece & m) {

    }

    void Sell::clientToObserveMode() {

    }

    void Sell::clientToSellMode(const SellerTerms & terms, uint32_t index) {

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
*/

}
}
}
