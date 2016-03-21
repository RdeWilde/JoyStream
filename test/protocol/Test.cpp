/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <Test.hpp>

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/ReadyForInvitation.hpp>
#include <protocol/statemachine/Buying.hpp>
#include <protocol/statemachine/Observing.hpp>
#include <protocol/statemachine/Invited.hpp>
#include <protocol/statemachine/WaitingToStart.hpp>
#include <protocol/statemachine/ReadyForPieceRequest.hpp>
#include <protocol/statemachine/ServicingPieceRequest.hpp>
#include <protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/MessageType.hpp>
#include <protocol/wire/JoiningContract.hpp>

#include <common/PrivateKey.hpp>
#include <common/typesafeOutPoint.hpp>
#include <StateMachineCallbackSpy.hpp>

using namespace joystream::protocol;

void Test::clientToObserveMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    statemachine::CBStateMachine * machine = spy.createFreshMachineInObserveMode();

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Observing).name());

    // Check that sell message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.message()->messageType(), wire::MessageType::observe);

    // Clean up machine
    delete machine;
}

void Test::clientToSellMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    SellerTerms terms(1,2,3,4,5);
    statemachine::CBStateMachine * machine = spy.createFreshMachineInSellMode(terms);

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::ReadyForInvitation).name());

    // Check that sell message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.message()->messageType(), wire::MessageType::sell);

    // Check that same terms have been sent
    QCOMPARE((static_cast<const wire::Sell *>(spy.message()))->terms(), terms);

    // Clean up machine
    delete machine;
}

void Test::clientToBuyMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    BuyerTerms terms(1,2,3,4,5);
    statemachine::CBStateMachine * machine = spy.createFreshMachineInBuyMode(terms);

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Buying).name());

    // Check that sell message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.message()->messageType(), wire::MessageType::buy);

    // Check that same terms have been sent
    QCOMPARE((static_cast<const wire::Buy *>(spy.message()))->terms(), terms);

    // Clean up machine
    delete machine;
}

void Test::peerToSellMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    statemachine::CBStateMachine * machine = spy.createFreshMachineInObserveMode();

    // Recieve mode message from peer
    SellerTerms terms(1,2,3,4,5);
    uint32_t index = 17;
    wire::Sell m(terms, index);
    machine->process_event(statemachine::event::Recv<wire::Sell>(&m));

    // Check that new peer state recorded is valid
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::sell);
    QCOMPARE(announced.sellModeTerms(), terms);
    QCOMPARE(announced.index(), index);

    // Clean up machine
    delete machine;
}

void Test::peerToBuyMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    statemachine::CBStateMachine * machine = spy.createFreshMachineInObserveMode();

    // Recieve mode message from peer
    BuyerTerms terms(1,2,3,4,5);
    wire::Buy m(terms);
    machine->process_event(statemachine::event::Recv<wire::Buy>(&m));

    // test deep history transition at various times?
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::buy);
    QCOMPARE(announced.buyModeTerms(), terms);

    // Clean up machine
    delete machine;
}

void Test::peerToObserveMode() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    statemachine::CBStateMachine * machine = spy.createFreshMachineInObserveMode();

    // Recieve mode message from peer
    wire::Observe m;
    machine->process_event(statemachine::event::Recv<wire::Observe>(&m));

    // test deep history transition at various times?
    QCOMPARE(machine->peerAnnouncedMode().modeAnnounced(), ModeAnnounced::observe);

    // Clean up machine
    delete machine;
}

void Test::selling() {

    StateMachineCallbackSpy spy;

    // Get machine into mode
    SellerTerms terms(1,2,3,4,5);
    statemachine::CBStateMachine * machine = spy.createFreshMachineInSellMode(terms);

    //// ReadyForInvitation state

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::ReadyForInvitation).name());

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    {
        const joystream::protocol::wire::ExtendedMessagePayload * m = spy.message();
        QCOMPARE(m->messageType(), wire::MessageType::sell);
        QCOMPARE((static_cast<const wire::Sell *>(m))->terms(), terms);
    }

    spy.reset();

    // Transition to Invited
    ContractInvitation invitation(1123,
                                  Coin::PrivateKey::generate().toPublicKey(),
                                  Coin::PrivateKey::generate().toPublicKey());

    // First try while peer is not buyer, which should cause exception to be thrown
    {
        wire::JoinContract m(invitation, 0);
        QVERIFY_EXCEPTION_THROWN(machine->process_event(statemachine::event::Recv<wire::JoinContract>(&m)),
                                 statemachine::exception::InvitedToJoinContractByNonBuyer);
    }

    spy.reset();

    // Create fresh machine in sell mode,
    // the exception destroys machine state
    delete machine;
    machine = spy.createFreshMachineInSellMode(terms);

    // Then have peer send buy message
    BuyerTerms buyerTerms(1,2,3,4,5);
    {
        wire::Buy m(buyerTerms);
        machine->process_event(statemachine::event::Recv<wire::Buy>(&m));
    }

    // and check
    QCOMPARE(machine->peerAnnouncedMode().modeAnnounced(), ModeAnnounced::buy);

    spy.reset();

    // Then try with wrong index
    {
        wire::JoinContract m(invitation, 31);
        machine->process_event(statemachine::event::Recv<wire::JoinContract>(&m));
    }

    // and check that failure callback is called for this
    QVERIFY(spy.hasBeenInvitedToOutdatedContract());

    spy.reset();

    // Then we do it with correct index,
    {
        wire::JoinContract m(invitation, 0);
        machine->process_event(statemachine::event::Recv<wire::JoinContract>(&m));
    }

    // and check that we are getting right invitation, and
    // transitioning to the Invited state
    QVERIFY(spy.hasBeenInvitedToJoinContract());
    QCOMPARE(spy.invitation(), invitation);
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Invited).name());

    spy.reset();

    //// Invited state

    // Transition to WaitingToStart state
    ContractRSVP rsvp(Coin::PrivateKey::generate().toPublicKey(),
                      Coin::PrivateKey::generate().toPublicKey());
    machine->process_event(statemachine::event::Joined(rsvp));

    // and that joining_contract message was sent with correct rsvp,
    // and we are in WaitingToStart state
    QVERIFY(spy.messageSent());
    {
        const joystream::protocol::wire::ExtendedMessagePayload * m = spy.message();
        QCOMPARE(m->messageType(), wire::MessageType::joining_contract);
        QCOMPARE((static_cast<const wire::JoiningContract *>(m))->rsvp(), rsvp);
    }
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::WaitingToStart).name());

    spy.reset();

    //// WaitingToStart state

    // Peer sends Ready message
    Coin::typesafeOutPoint anchor;
    {
        wire::Ready m(anchor);
        machine->process_event(statemachine::event::Recv<wire::Ready>(&m));
    }

    // and then check that callback is made with correct anchor,
    // and we are in ReadyForPieceRequest state
    QVERIFY(spy.contractHasBeenPrepared());
    QCOMPARE(spy.anchor(), anchor);
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::ReadyForPieceRequest).name());

    spy.reset();

    //// ReadyForPieceRequest state

    // Peer sends FullPieceRequest message
    int pieceIndex = 9;
    {
        wire::RequestFullPiece m(pieceIndex);
        machine->process_event(statemachine::event::Recv<wire::RequestFullPiece>(&m));
    }

    // and then check that callback is made with correct piece index,
    // and we are in ServicingPieceRequest
    QVERIFY(spy.pieceHasBeenRequested());
    QCOMPARE(spy.piece(), pieceIndex);
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::PieceRequested).name());

    spy.reset();

    //// ServicingPieceRequest state





    // Clean up machine
    delete machine;
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
