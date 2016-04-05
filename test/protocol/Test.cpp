/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <Test.hpp>

#include <CBStateMachineCallbackSpy.hpp>
#include <SellingNavigator.hpp>

#include <wire/Sell.hpp>
#include <wire/Buy.hpp>
#include <wire/Observe.hpp>
#include <wire/JoinContract.hpp>
#include <wire/JoiningContract.hpp>
#include <wire/Ready.hpp>
#include <wire/RequestFullPiece.hpp>
#include <wire/SellerTerms.hpp>
#include <wire/BuyerTerms.hpp>

#include <protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>
#include <protocol/statemachine/event/Joined.hpp>

void Test::observing() {

    CBStateMachineCallbackSpy spy;

    //// Create machine in ChooseMode state
    statemachine::CBStateMachine * machine = spy.createMonitoredMachine();

    // Issue client event to change mode
    machine->process_event(event::ObserveModeStarted());

    // Check that observe mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.message()->messageType(), joystream::wire::MessageType::observe);

    //// In Observing state

    // Have peer mode switch modes
    peerToSellMode(machine, joystream::wire::SellerTerms(1,2,3,4,5), 0);
    peerToBuyMode(machine, joystream::wire::BuyerTerms(1,2,3,4,5));
    peerToObserveMode(machine);

    // Clean up machine
    delete machine;
}

void Test::selling() {

    // Setup navigator and spy
    Coin::PrivateKey payorContractSk("E9873D79C6D87DC0FB6A5778633389F4");
    SellingNavigator::Fixture f;
    f.peerTerms = joystream::wire::BuyerTerms(1,2,3,4,5);
    f.clientTerms = joystream::wire::SellerTerms(1,2,3,4,5);
    f.invitation = joystream::wire::ContractInvitation(1123,
                                      payorContractSk.toPublicKey(),
                                      Coin::PubKeyHash("03a3fac91cac4a5c9ec870b444c4890ec7d68671"));
    SellingNavigator navigator(f);

    //// Create machine in ChooseMode state
    CBStateMachineCallbackSpy spy;
    statemachine::CBStateMachine * machine = spy.createMonitoredMachine();

    // Issue client event to change to sell mode
    machine->process_event(event::SellModeStarted(f.clientTerms));

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Sell *>(spy.message()))->terms(), f.clientTerms);

    spy.reset();

    //// All states below are substates of Selling state

    //// In ReadyForInvitation state

    // Peer invites us (seller) before announcing being in any mode
    // Check that this causes exception
    QVERIFY_EXCEPTION_THROWN(machine->process_event(statemachine::event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 0))),
                             statemachine::exception::InvitedToJoinContractByNonBuyer);

    spy.reset();

    // Recreate fresh machine in sell mode,
    // the prior exception destroys machine state
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));

    // Then have peer announce being a buyer
    peerToBuyMode(machine, f.peerTerms);

    spy.reset();

    // Then buyer peer invites us (seller) with incorrect index
    machine->process_event(statemachine::event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 31)));

    // Check that failure callback is made
    QVERIFY(spy.hasBeenInvitedToOutdatedContract());

    spy.reset();

    // Then buyer peer invites us (seller) with correct index
    machine->process_event(statemachine::event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 0)));

    // Check that we are getting right invitation
    QVERIFY(spy.hasBeenInvitedToJoinContract());
    QCOMPARE(spy.invitation(), f.invitation);
    //QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Invited).name());

    spy.reset();

    //// In Invited state

    // We (seller) respond by joining the contract
    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::PubKeyHash payeeFinalPkHash("7897447899567885436990976468990ec7d68671");

    machine->process_event(statemachine::event::Joined(payeeContractKeyPair, payeeFinalPkHash));

    // Check that joining_contract message was sent with correct rsvp
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::JoiningContract *>(spy.message()))->rsvp(),
             joystream::wire::ContractRSVP(payeeContractKeyPair.pk(), payeeFinalPkHash));

    spy.reset();

    //// In WaitingToStart state

    // Peer (buyer) announces being ready
    Coin::typesafeOutPoint anchor;
    machine->process_event(statemachine::event::Recv<joystream::wire::Ready>(new joystream::wire::Ready(anchor)));

    // Check that callback is made with correct anchor,
    QVERIFY(spy.contractHasBeenPrepared());
    QCOMPARE(spy.anchor(), anchor);
    //QCOMPARE(machine->getInnerStateName(), typeid(statemachine::ReadyForPieceRequest).name());

    spy.reset();

    //// In ReadyForPieceRequest state

    // Peer requests piece which is invalid
    machine->process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(9999)));

    // Check that invalid piece callback was made
    QVERIFY(spy.invalidPieceHasBeenRequested());

    spy.reset();

    // Last transition terminates machine, so
    // recreate fresh machine in ReadyForPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.fastForwardToReadyForPieceRequest(machine);
    spy.reset();

    // Peer request valid piece
    int validPieceIndexRequested = 1;
    machine->process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(validPieceIndexRequested)));

    // Check that callback is made with correct piece index,
    // and we are in ServicingPieceRequest
    QVERIFY(spy.pieceHasBeenRequested());
    QCOMPARE(spy.piece(), validPieceIndexRequested);

    spy.reset();

    //// Transition to ServicingPieceRequest state

    // Clean up machine
    delete machine;
}

void Test::buyerAndSellers() {

}

void Test::buying() {

    CBStateMachineCallbackSpy spy;

    // Create machine in ChooseMode state
    statemachine::CBStateMachine * machine = spy.createMonitoredMachine();

    joystream::wire::BuyerTerms terms(1,2,3,4,5);
    machine->process_event(event::BuyModeStarted(terms));

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message()))->terms(), terms);

    //// In Observing state

    // Clean up machine
    delete machine;
}


void Test::peerToSellMode(statemachine::CBStateMachine * machine, const joystream::wire::SellerTerms & terms, uint32_t index) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Sell>(new joystream::wire::Sell(terms, index)));

    // Check that new peer state recorded is valid
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::sell);
    QCOMPARE(announced.sellModeTerms(), terms);
    QCOMPARE(announced.index(), index);
}

void Test::peerToBuyMode(statemachine::CBStateMachine * machine, const joystream::wire::BuyerTerms & terms) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Buy>(new joystream::wire::Buy(terms)));

    // test deep history transition at various times?
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::buy);
    QCOMPARE(announced.buyModeTerms(), terms);
}

void Test::peerToObserveMode(statemachine::CBStateMachine * machine) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Observe>(new joystream::wire::Observe()));

    // test deep history transition at various times?
    QCOMPARE(machine->peerAnnouncedMode().modeAnnounced(), ModeAnnounced::observe);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
