/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#include <SellingNavigator.hpp>

event::Recv<protocol_wire::Payment> SellingNavigator::Fixture::goodPayment(const Coin::PrivateKey & payorContractSk, int numberOfPayments) const {

    joystream::paymentchannel::Payor payor(sellModeStarted.terms().minPrice(),
                                           numberOfPayments,
                                           contractReady.message().value(),
                                           sellModeStarted.terms().settlementFee(),
                                           sellModeStarted.terms().minLock(),
                                           contractReady.message().anchor(),
                                           Coin::KeyPair(payorContractSk),
                                           contractReady.message().finalPkHash(),
                                           joinedContract.contractKeys().pk(),
                                           joinedContract.finalPkHash());

    Coin::Signature payment = payor.generatePayorSettlementSignature();

    return event::Recv<protocol_wire::Payment>(payment);
}

SellingNavigator::SellingNavigator(Fixture fixture)
    : _fixture(fixture) {
}

void SellingNavigator::toSellMode(CBStateMachine * machine) {
    machine->processEvent(_fixture.sellModeStarted);
}

void SellingNavigator::toReadyForPieceRequest(CBStateMachine * machine) {
    machine->processEvent(_fixture.validJoinContract);
    machine->processEvent(_fixture.joinedContract);
    machine->processEvent(_fixture.contractReady);
}

void SellingNavigator::toLoadingPiece(CBStateMachine * machine) {
    toReadyForPieceRequest(machine);
    machine->processEvent(_fixture.validPieceRequest);
}

void SellingNavigator::toWaitingForPayment(CBStateMachine * machine) {
    toLoadingPiece(machine);
    machine->processEvent(_fixture.fullPiece);
}
