/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 31 2016
 */

#include <SellingNavigator.hpp>
#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/event/Joined.hpp>
#include <wire/Sell.hpp>
#include <wire/Buy.hpp>
#include <wire/Observe.hpp>
#include <wire/Ready.hpp>
#include <wire/JoinContract.hpp>

using namespace joystream::protocol::statemachine;

SellingNavigator::SellingNavigator(Fixture fixture)
    : _fixture(fixture) {
}

void SellingNavigator::toSellMode(statemachine::CBStateMachine *) {

}

void SellingNavigator::fastForwardToReadyForPieceRequest(CBStateMachine * machine) {
    //machine->process_event(event::Recv<wire::JoinContract>(new wire::JoinContract(_fixture., 0)));
    //machine->process_event(statemachine::event::Joined(payeeContractKeyPair, payeeFinalPkHash));
    //machine->process_event(statemachine::event::Recv<wire::Ready>(new wire::Ready(anchor)));
}

void SellingNavigator::fastForwardToLoadingPiece(statemachine::CBStateMachine * machine) {

}
