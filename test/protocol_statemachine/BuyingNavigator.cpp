/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#include <BuyingNavigator.hpp>
#include <protocol_statemachine/CBStateMachine.hpp>
#include <protocol_statemachine/event/BuyModeStarted.hpp>
#include <protocol_statemachine/event/InviteSeller.hpp>
#include <protocol_statemachine/event/ContractPrepared.hpp>
#include <protocol_statemachine/event/RequestPiece.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/FullPiece.hpp>
#include <paymentchannel/Payee.hpp>

BuyingNavigator::BuyingNavigator(const Fixture & fixture)
    : _fixture(fixture) {
}

void BuyingNavigator::toBuyMode(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(protocol_statemachine::event::BuyModeStarted(_fixture.clientTerms));
}

void BuyingNavigator::toSellerHasJoined(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::Sell>(new protocol_wire::Sell(_fixture.peerTerms, _fixture.index)));
    machine->process_event(protocol_statemachine::event::InviteSeller(_fixture.value, _fixture.buyerContractKeyPair, _fixture.finalPkHash));
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::JoiningContract>(new protocol_wire::JoiningContract(_fixture.rsvp)));
}

void BuyingNavigator::toProcessingPiece(protocol_statemachine::CBStateMachine * machine) {
    toSellerHasJoined(machine);
    machine->process_event(protocol_statemachine::event::ContractPrepared(_fixture.anchor));
    machine->process_event(protocol_statemachine::event::RequestPiece(_fixture.pieceIndex));
    machine->process_event(protocol_statemachine::event::Recv<protocol_wire::FullPiece>(new protocol_wire::FullPiece(_fixture.pieceData)));
}

bool BuyingNavigator::validatePayment(const Coin::Signature & payment, int numberOfPayments) const {

    joystream::paymentchannel::Payee payee(numberOfPayments,
                                           _fixture.peerTerms.minLock(),
                                           _fixture.peerTerms.minPrice(),
                                           _fixture.value,
                                           _fixture.peerTerms.settlementFee(),
                                           _fixture.clientTerms.refundFee(),
                                           _fixture.anchor,
                                           Coin::KeyPair(_fixture.rsvp.contractPk(), Coin::PrivateKey()),
                                           _fixture.rsvp.finalPkHash(),
                                           _fixture.buyerContractKeyPair.pk(),
                                           _fixture.finalPkHash,
                                           Coin::Signature());

    return payee.registerPayment(payment);
}
