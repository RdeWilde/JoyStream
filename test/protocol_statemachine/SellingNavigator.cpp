/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#include <SellingNavigator.hpp>
#include <protocol_statemachine/CBStateMachine.hpp>
#include <protocol_statemachine/event/SellModeStarted.hpp>
#include <protocol_statemachine/event/Joined.hpp>
#include <protocol_statemachine/event/PieceLoaded.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Ready.hpp>
#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/RequestFullPiece.hpp>

#include <paymentchannel/Payor.hpp>

SellingNavigator::SellingNavigator(Fixture fixture)
    : _fixture(fixture) {
}

void SellingNavigator::toSellMode(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(protocol_statemachine::event::SellModeStarted(_fixture.clientTerms));
}

void SellingNavigator::toReadyForPieceRequest(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::JoinContract>(new protocol_wire::JoinContract(_fixture.invitation, 0)));
    machine->process_event(protocol_statemachine::event::Joined(_fixture.payeeContractKeyPair, _fixture.payeeFinalPkHash));
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::Ready>(new protocol_wire::Ready(_fixture.anchor)));
}

void SellingNavigator::toLoadingPiece(protocol_statemachine::CBStateMachine * machine) {
    toReadyForPieceRequest(machine);
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::RequestFullPiece>(new protocol_wire::RequestFullPiece(_fixture.validPieceIndex)));
}

void SellingNavigator::toWaitingForPayment(protocol_statemachine::CBStateMachine * machine) {
    toLoadingPiece(machine);
    machine->process_event(protocol_statemachine::event::PieceLoaded(_fixture.data));
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

