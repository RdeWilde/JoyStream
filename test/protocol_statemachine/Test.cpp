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

void peerToSellMode(CBStateMachine * machine, const event::Recv<protocol_wire::Sell> & e) {

    // Recieve mode message from peer
    machine->processEvent(e);

    // Check that new peer state recorded is valid
    AnnouncedModeAndTerms announced = machine->announcedModeAndTermsFromPeer();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::sell);
    QCOMPARE(announced.sellModeTerms(), e.message().terms());
    QCOMPARE(announced.index(), e.message().index());
}

void peerToBuyMode(CBStateMachine * machine, const event::Recv<protocol_wire::Buy> & e) {

    // Recieve mode message from peer
    machine->processEvent(e);

    // test deep history transition at various times?
    AnnouncedModeAndTerms announced = machine->announcedModeAndTermsFromPeer();
    QCOMPARE(announced.modeAnnounced(), ModeAnnounced::buy);
    QCOMPARE(announced.buyModeTerms(), e.message().terms());
}

void peerToObserveMode(CBStateMachine * machine) {

    // Recieve mode message from peer
    machine->processEvent(event::Recv<protocol_wire::Observe>(protocol_wire::Observe()));

    // test deep history transition at various times?
    QCOMPARE(machine->announcedModeAndTermsFromPeer().modeAnnounced(), ModeAnnounced::observe);
}

void Test::observing() {

    CBStateMachineCallbackSpy spy;

    //// Create machine in ChooseMode state
    CBStateMachine * machine = spy.createMonitoredMachine();

    QVERIFY(machine->inState<ChooseMode>());

    // Issue client event to change mode
    machine->processEvent(event::ObserveModeStarted());

    // Check that observe mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::observe);

    //// In Observing state

    QVERIFY(machine->inState<Observing>());

    // Have peer mode switch modes
    peerToSellMode(machine, event::Recv<protocol_wire::Sell>(protocol_wire::Sell(protocol_wire::SellerTerms(1,2,3,4,5), 0)));
    peerToBuyMode(machine, event::Recv<protocol_wire::Buy>(protocol_wire::BuyerTerms(1,2,3,4,5)));
    peerToObserveMode(machine);

    // Clean up machine
    delete machine;
}

void Test::selling() {

    // Setup navigator and spy
    Coin::PrivateKey payorContractSk("E9873D79C6D87DC0FB6A5778633389F4");

    SellingNavigator::Fixture f;
    f.peerToBuyMode = event::Recv<protocol_wire::Buy>(protocol_wire::Buy(protocol_wire::BuyerTerms(1,2,3,4,5)));
    f.sellModeStarted = event::SellModeStarted(protocol_wire::SellerTerms(1,2,3,4,5));
    f.invalidJoinContract = event::Recv<protocol_wire::JoinContract>(protocol_wire::JoinContract(31));
    f.validJoinContract = event::Recv<protocol_wire::JoinContract>(protocol_wire::JoinContract(0));
    f.joinedContract = event::Joined(Coin::KeyPair(Coin::PrivateKey("b8aa30d8f1d398883f0eeb5079777c42")), Coin::PubKeyHash("31149292f8ba11da4aeb833f6cd8ae0650a82340"));
    f.contractReady = event::Recv<protocol_wire::Ready>(protocol_wire::Ready(1123,
                                                                             Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")), 55),
                                                                             payorContractSk.toPublicKey(),
                                                                             Coin::PubKeyHash("03a3fac91cac4a5c9ec870b444c4890ec7d68671")));
    f.invalidPieceRequest = event::Recv<protocol_wire::RequestFullPiece>(protocol_wire::RequestFullPiece(9999));
    f.validPieceRequest = event::Recv<protocol_wire::RequestFullPiece>(protocol_wire::RequestFullPiece(1));
    f.badPayment = event::Recv<protocol_wire::Payment>(protocol_wire::Payment(Coin::Signature("8185781409579048901234890234")));

    const char * rawData = "datadataajfkdløajdklføasjdklføadsjkfløasdjkfløadata";
    int rawDataLength = strlen(rawData);
    f.fullPiece = event::PieceLoaded(protocol_wire::PieceData(protocol_wire::PieceData(boost::shared_array<char>{new char [rawDataLength]}, rawDataLength)));

    // TODO: copy rawData into boost::shared_array!!!
    //for(int i = 0;i < rawDataLength;i++)
    //    f.fullPiece.pieceData().piece().operator [](i) = rawData[i];

    SellingNavigator navigator(f);

    //// Create machine in ChooseMode state
    CBStateMachineCallbackSpy spy;
    CBStateMachine * machine = spy.createMonitoredMachine();

    QVERIFY(machine->inState<ChooseMode>());

    // Issue client event to change to sell mode
    machine->processEvent(f.sellModeStarted);

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::sell);
    QCOMPARE(spy.sellMessage().terms(), f.sellModeStarted.terms());

    spy.reset();

    //// All states below are substates of Selling state

    std::cout << "--- In ReadyForInvitation state ---" << std::endl;

    // Client updates terms
    protocol_wire::SellerTerms newSellTerms(33, 123, 4, 1000, 1);
    machine->processEvent(event::UpdateTerms<protocol_wire::SellerTerms>(newSellTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::sell);
    QCOMPARE(spy.sellMessage().terms(), newSellTerms);

    // Peer invites us (seller), with valid index, but before announcing being in any mode
    // Check that this causes exception
    QVERIFY_EXCEPTION_THROWN(machine->processEvent(f.validJoinContract), exception::InvitedToJoinContractByNonBuyer);

    spy.reset();

    // Recreate fresh machine in sell mode,
    // the prior exception destroys machine state
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);

    // Then have peer announce being a buyer
    peerToBuyMode(machine, f.peerToBuyMode);

    spy.reset();

    // Then buyer peer invites us (seller) with incorrect index
    machine->processEvent(f.invalidJoinContract);

    // Check that failure callback is made
    QVERIFY(spy.hasBeenInvitedToOutdatedContract());

    spy.reset();

    // Then buyer peer invites us (seller) with correct index
    machine->processEvent(f.validJoinContract);

    // Check that we are getting right invitation
    QVERIFY(spy.hasBeenInvitedToJoinContract());
    QVERIFY(machine->inState<Invited>());

    spy.reset();

    std::cout << "--- In Invited state ---" << std::endl;

    // We (seller) respond by joining the contract
    machine->processEvent(f.joinedContract);

    // Check that joining_contract message was sent with correct rsvp
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::joining_contract);
    QCOMPARE(spy.joiningContractMessage().contractPk(), f.joinedContract.contractKeys().pk());
    QCOMPARE(spy.joiningContractMessage().finalPkHash(), f.joinedContract.finalPkHash());

    spy.reset();

    std::cout << "--- In WaitingToStart state ---" << std::endl;

    // Peer (buyer) announces being ready
    machine->processEvent(f.contractReady);

    // Check that callback is made with correct anchor,
    QVERIFY(spy.contractHasBeenPrepared());
    QCOMPARE(spy.anchor(), f.contractReady.message().anchor());
    QCOMPARE(spy.value(), f.contractReady.message().value());
    QCOMPARE(spy.contractPk(), f.contractReady.message().contractPk());
    QCOMPARE(spy.finalPkHash(), f.contractReady.message().finalPkHash());
    //QCOMPARE(machine->getInnerStateName(), typeid(ReadyForPieceRequest).name());

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Peer requests piece which is invalid
    machine->processEvent(f.invalidPieceRequest);

    // Check that invalid piece callback was made
    QVERIFY(spy.invalidPieceHasBeenRequested());

    spy.reset();

    // Last transition terminates machine, so
    // recreate fresh machine in ReadyForPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);
    peerToBuyMode(machine, f.peerToBuyMode);
    navigator.toReadyForPieceRequest(machine);
    spy.reset();

    // Peer request valid piece
    machine->processEvent(f.validPieceRequest);

    // Check that callback is made with correct piece index,
    // and we are in ServicingPieceRequest
    QVERIFY(spy.pieceHasBeenRequested());
    QCOMPARE(spy.piece(), f.validPieceRequest.message().pieceIndex());

    spy.reset();

    std::cout << "--- In ServicingPieceRequest state ---" << std::endl;

    // Check that peer announcing mode terminates machine
    peerToBuyMode(machine, f.peerToBuyMode);
    QVERIFY(spy.paymentInterrupted());

    // Check that peer announcing mode terminates machine
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);
    peerToBuyMode(machine, f.peerToBuyMode);
    navigator.toLoadingPiece(machine);
    spy.reset();

    peerToBuyMode(machine, f.peerToBuyMode);
    QVERIFY(spy.paymentInterrupted());

    // Check that peer announcing mode terminates machine
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);
    peerToBuyMode(machine, f.peerToBuyMode);
    navigator.toLoadingPiece(machine);
    spy.reset();

    peerToObserveMode(machine);
    QVERIFY(spy.paymentInterrupted());

    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);
    peerToBuyMode(machine, f.peerToBuyMode);
    navigator.toLoadingPiece(machine);
    spy.reset();

    machine->processEvent(f.fullPiece);

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::full_piece);
    QCOMPARE(spy.fullPieceMessage().pieceData(), f.fullPiece.pieceData());

    spy.reset();

    std::cout << "--- In WaitingForPayment state ---" << std::endl;

    // Have peer send invalid payment!
    machine->processEvent(f.badPayment);

    // Check that invalid piece callback was made
    QVERIFY(spy.receivedInvalidPayment());
    QCOMPARE(spy.invalidPaymentSignature(), f.badPayment.message().sig());

    // Have peer send valid payment
    // Last transition terminates machine, so
    // recreate fresh machine in ServicingPieceRequest state, and peer in buy mode
    delete machine;
    machine = spy.createMonitoredMachine();
    machine->processEvent(f.sellModeStarted);
    peerToBuyMode(machine, f.peerToBuyMode);
    navigator.toWaitingForPayment(machine);
    spy.reset();

    // Generate payor payment signature for first payment
    event::Recv<protocol_wire::Payment> e = f.goodPayment(payorContractSk, 1);
    machine->processEvent(e);

    // Check that invalid piece callback was made
    QVERIFY(spy.receivedValidPayment());
    QCOMPARE(spy.validPaymentSignature(), e.message().sig());

    spy.reset();

    std::cout << "--- In ReadyForPieceRequest state ---" << std::endl;

    // Client transition to Buy mode
    protocol_wire::BuyerTerms newBuyerTerms(7, 7, 7, 7, 7);
    machine->processEvent(event::BuyModeStarted(newBuyerTerms));

    // Check that mode message was sent
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::buy);
    QCOMPARE(spy.buyMessage().terms(), newBuyerTerms);

    // Clean up machine
    delete machine;
}

void Test::buying() {

    BuyingNavigator::Fixture f;
    f.peerToSellMode = event::Recv<protocol_wire::Sell>(protocol_wire::Sell(protocol_wire::SellerTerms(11,22,33,44,55), 5));
    f.buyModeStarted = event::BuyModeStarted(protocol_wire::BuyerTerms(88,77,66,99,111));
    f.inviteSeller = event::InviteSeller();
    f.joiningContract = event::Recv<protocol_wire::JoiningContract>(protocol_wire::JoiningContract(Coin::PublicKey(uchar_vector("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8")),
                                                                                                   Coin::PubKeyHash("892131b6cbf303692785db2c607fb915ae622203")));
    f.contractPrepared = event::ContractPrepared(Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("eee2b334dd735dac60ae57893c2528087fd3d386b57cac42f4e6ace6403f16b3")), 78),
                                                 Coin::KeyPair(Coin::PrivateKey("153213303DA61F20BD67FC233AA33262")),
                                                 Coin::PubKeyHash("3457b36d53494fb1ce39a4500d76373da994585e"),
                                                 2222);
    f.requestPiece = event::RequestPiece(1);

    const char * rawData = "datadataajfkdløajdklføasjdklføadsjkfløasdjkfløadata";
    int rawDataLength = strlen(rawData);
    f.fullPiece = event::Recv<protocol_wire::FullPiece>(protocol_wire::PieceData(boost::shared_array<char>{new char [rawDataLength]}, rawDataLength));

    BuyingNavigator navigator(f);

    //// Create machine in ChooseMode state
    CBStateMachineCallbackSpy spy;
    CBStateMachine * machine = spy.createMonitoredMachine();

    QVERIFY(machine->inState<ChooseMode>());

    // Client changes to buy mode
    navigator.toBuyMode(machine);

    // Check that sell message was sent with correct terms
    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::buy);
    QCOMPARE(spy.buyMessage().terms(), f.buyModeStarted.terms());

    spy.reset();

    //// All states below are substates of Buying state

    std::cout << "--- In ReadyToInviteSeller state ---" << std::endl;

    // Client, incorrectly, tries to invite peer
    // which is in buy mode, which should result in exception
    peerToBuyMode(machine, event::Recv<protocol_wire::Buy>(protocol_wire::BuyerTerms(0,0,0,0,0)));
    QVERIFY_EXCEPTION_THROWN(machine->processEvent(f.inviteSeller), exception::CannotInviteNonSeller);

    // Recreate fresh machine in sell mode,
    // the prior exception destroys machine state
    delete machine;
    machine = spy.createMonitoredMachine();
    navigator.toBuyMode(machine);
    peerToSellMode(machine, f.peerToSellMode);

    spy.reset();

    // Invite seller peer
    machine->processEvent(f.inviteSeller);

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::join_contract);
    //QCOMPARE(spy.joinContractMessage().index(), machine->pe

    spy.reset();

    std::cout << "--- In WaitingForSellerToJoin state ---" << std::endl;

    // Peer joins contract
    machine->processEvent(f.joiningContract);

    QVERIFY(spy.sellerHasJoined());

    spy.reset();

    std::cout << "--- In PreparingContract state ---" << std::endl;

    // Peer interrupts contract by announcing sell mode
    peerToSellMode(machine, f.peerToSellMode);

    QVERIFY(spy.sellerHasInterruptedContract());

    spy.reset();

    // Peer interrupts contract by announcing buy mode: currently in ReadyToInviteSeller
    navigator.toSellerHasJoined(machine);
    spy.reset();

    peerToBuyMode(machine, event::Recv<protocol_wire::Buy>(protocol_wire::BuyerTerms()));

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

    machine->processEvent(f.contractPrepared);

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::ready);
    QCOMPARE(spy.readyMessage().anchor(), f.contractPrepared.anchor());
    QCOMPARE(spy.readyMessage().value(), f.contractPrepared.value());
    QCOMPARE(spy.readyMessage().contractPk(), f.contractPrepared.contractKeyPair().pk());
    QCOMPARE(spy.readyMessage().finalPkHash(), f.contractPrepared.finalPkHash());

    spy.reset();

    std::cout << "--- In ReadyToRequestPiece state ---" << std::endl;

    // Client requests a piece
    machine->processEvent(f.requestPiece);

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::request_full_piece);
    QCOMPARE(spy.requestFullPieceMessage().pieceIndex(), f.requestPiece.pieceIndex());

    spy.reset();

    std::cout << "--- In WaitingForFullPiece state ---" << std::endl;

    // Peer sends piece back to client
    machine->processEvent(f.fullPiece);

    QVERIFY(spy.hasReceivedFullPiece());
    QCOMPARE(spy.pieceData(), f.fullPiece.message().pieceData());

    spy.reset();

    std::cout << "--- In ProcessingPiece state ---" << std::endl;

    // Client says piece was valid, and payment is sent
    machine->processEvent(event::SendPayment());

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::payment);
    QVERIFY(f.validatePayment(spy.paymentMessage().sig(), 0));

    spy.reset();

    // Update terms
    protocol_wire::BuyerTerms testTerms(4123,65436,1432,652,77777);
    machine->processEvent(event::UpdateTerms<protocol_wire::BuyerTerms>(testTerms));

    QVERIFY(spy.messageSent());
    QCOMPARE(spy.messageType(), protocol_wire::MessageType::buy);
    QCOMPARE(spy.buyMessage().terms(), testTerms);

    spy.reset();

    // Clean up machine
    delete machine;

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
