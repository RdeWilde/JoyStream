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
#include <wire/FullPiece.hpp>
#include <wire/Payment.hpp>
#include <protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>
#include <protocol/statemachine/event/Joined.hpp>
#include <protocol/statemachine/event/PieceLoaded.hpp>
#include <protocol/statemachine/event/UpdateTerms.hpp>

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
    f.invitation = joystream::wire::ContractInvitation(1123, payorContractSk.toPublicKey(), Coin::PubKeyHash("03a3fac91cac4a5c9ec870b444c4890ec7d68671"));
    f.payeeContractKeyPair = Coin::KeyPair(Coin::PrivateKey("b8aa30d8f1d398883f0eeb5079777c42"));
    f.payeeFinalPkHash = Coin::PubKeyHash("31149292f8ba11da4aeb833f6cd8ae0650a82340");
    f.anchor = Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")), 55);
    f.invalidPieceIndex = 9999;
    f.validPieceIndex = 1;
    const char * rawData = "datadataajfkdløajdklføasjdklføadsjkfløasdjkfløadata";
    int rawDataLength = strlen(rawData);
    f.data = joystream::wire::PieceData(boost::shared_array<char>{new char [rawDataLength]}, rawDataLength);

    // TODO: copy rawData into boost::shared_array!!!

    SellingNavigator navigator(f);

    //// Create machine in ChooseMode state
    CBStateMachineCallbackSpy spy;
    statemachine::CBStateMachine * machine = spy.createMonitoredMachine();

    // Issue client event to change to sell mode
    machine->process_event(event::SellModeStarted(f.clientTerms));

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Sell *>(spy.message().get()))->terms(), f.clientTerms);

    spy.reset();

    //// All states below are substates of Selling state

    std::cout << "--- In ReadyForInvitation state ---" << std::endl;

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

    std::cout << "--- In Invited state ---" << std::endl;

    // We (seller) respond by joining the contract
    machine->process_event(statemachine::event::Joined(f.payeeContractKeyPair, f.payeeFinalPkHash));

    // Check that joining_contract message was sent with correct rsvp
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::JoiningContract *>(spy.message().get()))->rsvp(),
             joystream::wire::ContractRSVP(f.payeeContractKeyPair.pk(), f.payeeFinalPkHash));

    spy.reset();

    std::cout << "--- In WaitingToStart state ---" << std::endl;

    // Peer (buyer) announces being ready
    Coin::typesafeOutPoint anchor;
    machine->process_event(statemachine::event::Recv<joystream::wire::Ready>(new joystream::wire::Ready(anchor)));

    // Check that callback is made with correct anchor,
    QVERIFY(spy.contractHasBeenPrepared());
    QCOMPARE(spy.anchor(), anchor);
    //QCOMPARE(machine->getInnerStateName(), typeid(statemachine::ReadyForPieceRequest).name());

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Peer requests piece which is invalid
    machine->process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(f.invalidPieceIndex)));

    // Check that invalid piece callback was made
    QVERIFY(spy.invalidPieceHasBeenRequested());

    spy.reset();

    // Last transition terminates machine, so
    // recreate fresh machine in ReadyForPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.toReadyForPieceRequest(machine);
    spy.reset();

    // Peer request valid piece
    machine->process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(f.validPieceIndex)));

    // Check that callback is made with correct piece index,
    // and we are in ServicingPieceRequest
    QVERIFY(spy.pieceHasBeenRequested());
    QCOMPARE(spy.piece(), f.validPieceIndex);

    spy.reset();

    std::cout << "--- In ServicingPieceRequest state ---" << std::endl;

    // Check that peer announcing mode terminates machine
    peerToBuyMode(machine, f.peerTerms);
    QVERIFY(spy.paymentInterrupted());

    // Check that peer announcing mode terminates machine
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.toLoadingPiece(machine);
    spy.reset();

    peerToSellMode(machine, f.clientTerms, 0);
    QVERIFY(spy.paymentInterrupted());

    // Check that peer announcing mode terminates machine
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.toLoadingPiece(machine);
    spy.reset();

    peerToObserveMode(machine);
    QVERIFY(spy.paymentInterrupted());

    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.toLoadingPiece(machine);
    spy.reset();

    machine->process_event(event::PieceLoaded(f.data));

    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::FullPiece *>(spy.message().get()))->pieceData(), f.data);

    spy.reset();

    std::cout << "--- In WaitingForPayment state ---" << std::endl;

    // Have peer send invalid payment!
    Coin::Signature badPaymentSignture("8185781409579048901234890234");
    machine->process_event(statemachine::event::Recv<joystream::wire::Payment>(new joystream::wire::Payment(badPaymentSignture)));

    // Check that invalid piece callback was made
    QVERIFY(spy.receivedInvalidPayment());
    QCOMPARE(spy.invalidPaymentSignature(), badPaymentSignture);

    // Have peer send valid payment
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->process_event(event::SellModeStarted(f.clientTerms));
    peerToBuyMode(machine, f.peerTerms);
    navigator.toWaitingForPayment(machine);
    spy.reset();

    // Generate payor payment signature for first payment
    Coin::Signature goodPaymentSignture = navigator.payorSignature(payorContractSk, 1);
    machine->process_event(statemachine::event::Recv<joystream::wire::Payment>(new joystream::wire::Payment(goodPaymentSignture)));

    // Check that invalid piece callback was made
    QVERIFY(spy.receivedValidPayment());
    QCOMPARE(spy.validPaymentSignature(), goodPaymentSignture);

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Peer requests piece which is invalid
    joystream::wire::SellerTerms newSellTerms(33, 123, 4, 1000, 1);
    machine->process_event(statemachine::event::UpdateTerms<joystream::wire::SellerTerms>(newSellTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Sell *>(spy.message().get()))->terms(), newSellTerms);

    std::cout << "--- In ReadyForInvitation state ---" << std::endl;

    // Client transition to Buy mode
    joystream::wire::BuyerTerms newBuyerTerms(7, 7, 7, 7, 7);
    machine->process_event(statemachine::event::BuyModeStarted(newBuyerTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message().get()))->terms(), newBuyerTerms);

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
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message().get()))->terms(), terms);

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
