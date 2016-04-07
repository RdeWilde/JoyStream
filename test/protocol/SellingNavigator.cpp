/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#include <SellingNavigator.hpp>
#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/event/SellModeStarted.hpp>
#include <protocol/statemachine/event/Joined.hpp>
#include <protocol/statemachine/event/PieceLoaded.hpp>
#include <wire/Sell.hpp>
#include <wire/Buy.hpp>
#include <wire/Observe.hpp>
#include <wire/Ready.hpp>
#include <wire/JoinContract.hpp>
#include <wire/RequestFullPiece.hpp>

#include <paymentchannel/Payor.hpp>

using namespace joystream::protocol::statemachine;

SellingNavigator::SellingNavigator(Fixture fixture)
    : _fixture(fixture) {
}

void SellingNavigator::toSellMode(statemachine::CBStateMachine * machine) {
    machine->process_event(event::SellModeStarted(_fixture.clientTerms));
}

void SellingNavigator::toReadyForPieceRequest(CBStateMachine * machine) {
    machine->process_event(event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(_fixture.invitation, 0)));
    machine->process_event(statemachine::event::Joined(_fixture.payeeContractKeyPair, _fixture.payeeFinalPkHash));
    machine->process_event(statemachine::event::Recv<joystream::wire::Ready>(new joystream::wire::Ready(_fixture.anchor)));
}

void SellingNavigator::toLoadingPiece(statemachine::CBStateMachine * machine) {
    toReadyForPieceRequest(machine);
    machine->process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(_fixture.validPieceIndex)));
}

void SellingNavigator::toWaitingForPayment(statemachine::CBStateMachine * machine) {
    toLoadingPiece(machine);
    machine->process_event(statemachine::event::PieceLoaded(_fixture.data));
}

Coin::Signature SellingNavigator::payorSignature(const Coin::PrivateKey & payorContractSk, int numberOfPayments) const {

    joystream::paymentchannel::Payor payor(_fixture.clientTerms.minPrice(),
                                             numberOfPayments,
                                             _fixture.invitation.value(),
                                             _fixture.peerTerms.refundFee(),
                                             _fixture.clientTerms.settlementFee(),
                                             _fixture.clientTerms.minLock(),
                                             _fixture.anchor,
                                             Coin::KeyPair(payorContractSk),
                                             _fixture.invitation.finalPkHash(),
                                             _fixture.payeeContractKeyPair.pk(),
                                             _fixture.payeeFinalPkHash,
                                             Coin::Signature(),
                                             Coin::Signature());

    return payor.generatePayorSettlementSignature();
}

