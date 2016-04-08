/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#include <BuyingNavigator.hpp>
#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/event/BuyModeStarted.hpp>
#include <protocol/statemachine/event/InviteSeller.hpp>
#include <protocol/statemachine/event/ContractPrepared.hpp>
#include <protocol/statemachine/event/RequestPiece.hpp>
#include <wire/Sell.hpp>
#include <wire/JoiningContract.hpp>
#include <wire/FullPiece.hpp>
#include <paymentchannel/Payee.hpp>

BuyingNavigator::BuyingNavigator(const Fixture & fixture)
    : _fixture(fixture) {
}

void BuyingNavigator::toBuyMode(CBStateMachine * machine) {
    machine->process_event(event::BuyModeStarted(_fixture.clientTerms));
}

void BuyingNavigator::toSellerHasJoined(CBStateMachine * machine) {
    machine->process_event(event::Recv<joystream::wire::Sell>(new joystream::wire::Sell(_fixture.peerTerms, _fixture.index)));
    machine->process_event(event::InviteSeller(_fixture.value, _fixture.buyerContractKeyPair, _fixture.finalPkHash));
    machine->process_event(event::Recv<joystream::wire::JoiningContract>(new joystream::wire::JoiningContract(_fixture.rsvp)));
}

void BuyingNavigator::toProcessingPiece(CBStateMachine * machine) {
    toSellerHasJoined(machine);
    machine->process_event(event::ContractPrepared(_fixture.anchor));
    machine->process_event(event::RequestPiece(_fixture.pieceIndex));
    machine->process_event(event::Recv<joystream::wire::FullPiece>(new joystream::wire::FullPiece(_fixture.pieceData)));
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
