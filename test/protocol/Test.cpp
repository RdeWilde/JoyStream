/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <Test.hpp>

#include <CBStateMachineCallbackSpy.hpp>
#include <SellingNavigator.hpp>
#include <BuyingNavigator.hpp>
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
#include <protocol/statemachine/event/InviteSeller.hpp>
#include <protocol/statemachine/exception/CannotInviteNonSeller.hpp>
#include <protocol/statemachine/event/ContractPrepared.hpp>
#include <protocol/statemachine/event/RequestPiece.hpp>
#include <protocol/statemachine/event/SendPayment.hpp>

void Test::observing() {

    CBStateMachineCallbackSpy spy;

    //// Create machine in ChooseMode state
    CBStateMachine * machine = spy.createMonitoredMachine();

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
    CBStateMachine * machine = spy.createMonitoredMachine();

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
    QVERIFY_EXCEPTION_THROWN(machine->process_event(event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 0))),
                             exception::InvitedToJoinContractByNonBuyer);

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
    machine->process_event(event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 31)));

    // Check that failure callback is made
    QVERIFY(spy.hasBeenInvitedToOutdatedContract());

    spy.reset();

    // Then buyer peer invites us (seller) with correct index
    machine->process_event(event::Recv<joystream::wire::JoinContract>(new joystream::wire::JoinContract(f.invitation, 0)));

    // Check that we are getting right invitation
    QVERIFY(spy.hasBeenInvitedToJoinContract());
    QCOMPARE(spy.invitation(), f.invitation);
    //QCOMPARE(machine->getInnerStateName(), typeid(Invited).name());

    spy.reset();

    std::cout << "--- In Invited state ---" << std::endl;

    // We (seller) respond by joining the contract
    machine->process_event(event::Joined(f.payeeContractKeyPair, f.payeeFinalPkHash));

    // Check that joining_contract message was sent with correct rsvp
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::JoiningContract *>(spy.message().get()))->rsvp(),
             joystream::wire::ContractRSVP(f.payeeContractKeyPair.pk(), f.payeeFinalPkHash));

    spy.reset();

    std::cout << "--- In WaitingToStart state ---" << std::endl;

    // Peer (buyer) announces being ready
    Coin::typesafeOutPoint anchor;
    machine->process_event(event::Recv<joystream::wire::Ready>(new joystream::wire::Ready(anchor)));

    // Check that callback is made with correct anchor,
    QVERIFY(spy.contractHasBeenPrepared());
    QCOMPARE(spy.anchor(), anchor);
    //QCOMPARE(machine->getInnerStateName(), typeid(ReadyForPieceRequest).name());

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Peer requests piece which is invalid
    machine->process_event(event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(f.invalidPieceIndex)));

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
    machine->process_event(event::Recv<joystream::wire::RequestFullPiece>(new joystream::wire::RequestFullPiece(f.validPieceIndex)));

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
    machine->process_event(event::Recv<joystream::wire::Payment>(new joystream::wire::Payment(badPaymentSignture)));

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
    machine->process_event(event::Recv<joystream::wire::Payment>(new joystream::wire::Payment(goodPaymentSignture)));

    // Check that invalid piece callback was made
    QVERIFY(spy.receivedValidPayment());
    QCOMPARE(spy.validPaymentSignature(), goodPaymentSignture);

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Peer requests piece which is invalid
    joystream::wire::SellerTerms newSellTerms(33, 123, 4, 1000, 1);
    machine->process_event(event::UpdateTerms<joystream::wire::SellerTerms>(newSellTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Sell *>(spy.message().get()))->terms(), newSellTerms);

    std::cout << "--- In ReadyForInvitation state ---" << std::endl;

    // Client transition to Buy mode
    joystream::wire::BuyerTerms newBuyerTerms(7, 7, 7, 7, 7);
    machine->process_event(event::BuyModeStarted(newBuyerTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message().get()))->terms(), newBuyerTerms);

    // Clean up machine
    delete machine;
}

void Test::buyerAndSellers() {

}

void Test::buying() {

    BuyingNavigator::Fixture f;
    f.peerTerms = joystream::wire::SellerTerms(11,22,33,44,55);
    f.index = 5;
    f.clientTerms = joystream::wire::BuyerTerms(88,77,66,99,111);
    f.value = 2222;
    f.buyerContractKeyPair = Coin::KeyPair(Coin::PrivateKey("153213303DA61F20BD67FC233AA33262"));
    f.finalPkHash = Coin::PubKeyHash("3457b36d53494fb1ce39a4500d76373da994585e");
    f.rsvp = joystream::wire::ContractRSVP(Coin::PublicKey(uchar_vector("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8")),
                                           Coin::PubKeyHash("892131b6cbf303692785db2c607fb915ae622203"));
    f.anchor = Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("eee2b334dd735dac60ae57893c2528087fd3d386b57cac42f4e6ace6403f16b3")), 78);
    f.pieceData = joystream::wire::PieceData();

    BuyingNavigator navigator(f);

    //// Create machine in ChooseMode state
    CBStateMachineCallbackSpy spy;
    CBStateMachine * machine = spy.createMonitoredMachine();

    // Client changes to buy mode
    navigator.toBuyMode(machine);

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message().get()))->terms(), f.clientTerms);

    spy.reset();

    //// All states below are substates of Buying state

    std::cout << "--- In ReadyToInviteSeller state ---" << std::endl;

    // Client, incorrectly, tries to invite peer
    // which is in buy mode, which should result in exception
    peerToBuyMode(machine, joystream::wire::BuyerTerms());
    QVERIFY_EXCEPTION_THROWN(machine->process_event(event::InviteSeller(0, Coin::KeyPair(), Coin::PubKeyHash())),
                             exception::CannotInviteNonSeller);

    // Recreate fresh machine in sell mode,
    // the prior exception destroys machine state
    delete machine;
    machine = spy.createMonitoredMachine();
    navigator.toBuyMode(machine);
    peerToSellMode(machine, f.peerTerms, f.index);

    spy.reset();

    // Invite seller peer
    machine->process_event(event::InviteSeller(f.value, f.buyerContractKeyPair, f.finalPkHash));

    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::JoinContract *>(spy.message().get()))->index(), f.index);

    spy.reset();

    std::cout << "--- In WaitingForSellerToJoin state ---" << std::endl;

    // Peer joins contract
    machine->process_event(event::Recv<joystream::wire::JoiningContract>(new joystream::wire::JoiningContract(f.rsvp)));

    QVERIFY(spy.sellerHasJoined());

    spy.reset();

    std::cout << "--- In PreparingContract state ---" << std::endl;

    // Peer interrupts contract by announcing sell mode
    peerToSellMode(machine, joystream::wire::SellerTerms(), 0);

    QVERIFY(spy.sellerHasInterruptedContract());

    spy.reset();

    // Peer interrupts contract by announcing buy mode: currently in ReadyToInviteSeller
    navigator.toSellerHasJoined(machine);
    spy.reset();

    peerToBuyMode(machine, joystream::wire::BuyerTerms());

    QVERIFY(spy.sellerHasInterruptedContract());

    spy.reset();

    // Peer interrupts contract by announcing observe mode: currently in ReadyToInviteSeller
    navigator.toSellerHasJoined(machine);
    spy.reset();

    peerToObserveMode(machine);

    QVERIFY(spy.sellerHasInterruptedContract());

    spy.reset();

    // Client announces contract as ready
    navigator.toSellerHasJoined(machine);
    spy.reset();

    machine->process_event(event::ContractPrepared(f.anchor));

    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Ready *>(spy.message().get()))->anchor(), f.anchor);

    spy.reset();

    std::cout << "--- In ReadyToRequestPiece state ---" << std::endl;

    // Client requests a piece
    machine->process_event(event::RequestPiece(f.pieceIndex));

    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::RequestFullPiece *>(spy.message().get()))->pieceIndex(), f.pieceIndex);

    spy.reset();

    std::cout << "--- In WaitingForFullPiece state ---" << std::endl;

    // Peer sends piece back to client
    machine->process_event(event::Recv<joystream::wire::FullPiece>(new joystream::wire::FullPiece(f.pieceData)));

    QVERIFY(spy.hasReceivedFullPiece());
    QCOMPARE(spy.pieceData(), f.pieceData);

    spy.reset();

    std::cout << "--- In ProcessingPiece state ---" << std::endl;

    // Client says piece was valid, and payment is sent
    machine->process_event(event::SendPayment());

    QVERIFY(spy.messageSent());
    QVERIFY(navigator.validatePayment((static_cast<const joystream::wire::Payment *>(spy.message().get()))->sig(), 0));

    spy.reset();

    // Update terms
    joystream::wire::BuyerTerms testTerms(4123,65436,1432,652,77777);
    machine->process_event(event::UpdateTerms<joystream::wire::BuyerTerms>(testTerms));

    QVERIFY(spy.messageSent());
    QCOMPARE((static_cast<const joystream::wire::Buy *>(spy.message().get()))->terms(), testTerms);

    spy.reset();

    // Clean up machine
    delete machine;
}

void Test::peerToSellMode(CBStateMachine * machine, const joystream::wire::SellerTerms & terms, uint32_t index) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Sell>(new joystream::wire::Sell(terms, index)));

    // Check that new peer state recorded is valid
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::sell);
    QCOMPARE(announced.sellModeTerms(), terms);
    QCOMPARE(announced.index(), index);
}

void Test::peerToBuyMode(CBStateMachine * machine, const joystream::wire::BuyerTerms & terms) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Buy>(new joystream::wire::Buy(terms)));

    // test deep history transition at various times?
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::buy);
    QCOMPARE(announced.buyModeTerms(), terms);
}

void Test::peerToObserveMode(CBStateMachine * machine) {

    // Recieve mode message from peer
    machine->process_event(event::Recv<joystream::wire::Observe>(new joystream::wire::Observe()));

    // test deep history transition at various times?
    QCOMPARE(machine->peerAnnouncedMode().modeAnnounced(), ModeAnnounced::observe);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
