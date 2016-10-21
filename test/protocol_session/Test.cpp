/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#include <Test.hpp>
#include <thread>
#include <common/Seed.hpp>
#include <CoinCore/hdkeys.h>

namespace joystream {
namespace protocol_session {
    template<>
    std::string IdToString<ID>(const ID & s) {
        return std::to_string(s);
    }
}}

Test::Test()
    : network(Coin::Network::testnet3)
    , session(nullptr)
    , spy(nullptr) {
}

void Test::init() {

    // Reset next key counter
    nextKey = 1;

    session = new Session<ID>();

    spy = new
    SessionSpy<ID>(
    [this](const P2SHScriptGeneratorFromPubKey & scriptGenerator, const uchar_vector &optional_data) -> Coin::KeyPair {

        Coin::PrivateKey sk = nextPrivateKey();

        scriptGenerator(sk.toPublicKey());

        return Coin::KeyPair(sk);
    },
    [this](int n) {

        std::vector<Coin::P2PKHAddress> addresses;

        for(int i = 0;i < n;i++){

            Coin::P2PKHAddress addr(this->network, nextPrivateKey().toPublicKey().toPubKeyHash());
            addresses.push_back(addr);
        }
        return addresses;

    },
    [this](int n) {

        std::vector<Coin::P2PKHAddress> addresses;

        for(int i = 0;i < n;i++){

            Coin::P2PKHAddress addr(this->network, nextPrivateKey().toPublicKey().toPubKeyHash());
            addresses.push_back(addr);
        }
        return addresses;

    },
    [this](const Coin::Transaction &) {
        return true;
    },
    session);
}

void Test::cleanup() {
    delete session;
    delete spy;

    session = nullptr;
    spy = nullptr;
}

void Test::observing() {

    // to observe mode
    toObserveMode();

    // incorrectly try to go to same mode again
    QVERIFY_EXCEPTION_THROWN(toObserveMode(), exception::SessionAlreadyInThisMode);

    // Start session
    firstStart();

    // incorrectly try to start again
    QVERIFY_EXCEPTION_THROWN(firstStart(), exception::StateIncompatibleOperation);

    // Do basic tests
    basic();

    //// Do transitions to other modes: buy, back to observe, sell

    // Have some new peers join, without announcing mode,
    // and transition to buy mode
    addConnection(0);
    addConnection(1);

    // Go to buy mode
    toBuyMode(Coin::UnspentOutputSet(),
              BuyingPolicy(),
              protocol_wire::BuyerTerms(),
              TorrentPieceInformation());

    // back to observe mode
    toObserveMode();

    // Go to sell mode
    toSellMode(SellingPolicy(),
               protocol_wire::SellerTerms(),
               0);

}

void Test::selling_basic() {

    // to observe mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(22, 134, 1, 88, 32), 0);

    // incorrectly try to go to same mode again
    QVERIFY_EXCEPTION_THROWN(toSellMode(SellingPolicy(), protocol_wire::SellerTerms(), 0),
                             exception::SessionAlreadyInThisMode);

    // Start session
    firstStart();

    // incorrectly try to start again
    QVERIFY_EXCEPTION_THROWN(firstStart(), exception::StateIncompatibleOperation);

    // Do basic tests
    basic();

    //// Do transitions to other modes: buy, back to sell, observe

    // Have some new peers join, without announcing mode,
    // and transition to buy mode
    addConnection(0);
    addConnection(1);

    // Go to buy mode
    toBuyMode(Coin::UnspentOutputSet(),
              BuyingPolicy(),
              protocol_wire::BuyerTerms(),
              TorrentPieceInformation());

    // back to sell mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(22, 134, 1, 88, 32), 0);

    // Go to observe mode
    toObserveMode();

}

void Test::selling() {

    protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);
    protocol_wire::BuyerTerms buyerTerms(24, 200, 2, 400, 5);
    Coin::PrivateKey payorContractSk("E9873D79C6D87DC0FB6A5778633389F4");
    protocol_wire::Ready ready(1123,
                               Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")), 55),
                               payorContractSk.toPublicKey(),
                               Coin::RedeemScriptHash::fromRawHash(uchar_vector("03a3fac91cac4a5c9ec870b444c4890ec7d68671")));

    ID peer = 0;
    uint numberOfExchangesWhileStarted = 5;

    assert(sellerTerms.satisfiedBy(buyerTerms));

    // back to sell mode
    toSellMode(SellingPolicy(), sellerTerms, numberOfExchangesWhileStarted + 1);

    // Start session
    firstStart();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    Coin::PublicKey payeeContractPk;
    Coin::RedeemScriptHash payeeFinalScriptHash;
    addBuyerAndGoToReadyForPieceRequest(peer, buyerTerms, ready, payeeContractPk, payeeFinalScriptHash);

    // Do sequence of data for payments exchanges
    paymentchannel::Payor payor = getPayor(sellerTerms, ready, payorContractSk, payeeContractPk, payeeFinalScriptHash);

    exchangeDataForPayment(peer, numberOfExchangesWhileStarted, payor);

    // Receive a single request
    receiveValidFullPieceRequest(peer, numberOfExchangesWhileStarted);

    // Pause
    pause();

    // Load piece, without it being sent
    protocol_wire::PieceData data;
    session->pieceLoaded(peer, data, numberOfExchangesWhileStarted);

    QVERIFY(spy->blankSession());
    ConnectionSpy<ID> * c = spy->connectionSpies.at(peer);
    QCOMPARE((int)c->sendMessageOnConnectionCallbackSlot.size(), 0);

    // Start up again
    session->start();

    // Make sure this results in piece being sent
    assertFullPieceSent(peer, data);
    spy->reset();

    // Update terms
    protocol_wire::SellerTerms newTerms;
    session->updateTerms(newTerms);

    // Check that peers were sent
    assertTermsSentToAllPeers();

    spy->reset();
}

void Test::selling_buyer_invited_with_bad_terms() {

    protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);
    protocol_wire::BuyerTerms badBuyerTerms;
    ID peer = 0;

    assert(!sellerTerms.satisfiedBy(badBuyerTerms));

    // back to sell mode
    toSellMode(SellingPolicy(), sellerTerms, 0);

    // Start session
    firstStart();

    // Have some new peers join, without announcing mode,
    addConnection(peer);

    // peer announces buyer mode
    session->processMessageOnConnection(peer, protocol_wire::Buy(badBuyerTerms));

    QVERIFY(spy->blank());

    // peer (buyer) sends invitation to terms which are not good enough
    session->processMessageOnConnection(peer, protocol_wire::JoinContract(0));

    QVERIFY(spy->onlyCalledRemovedConnection());
    QCOMPARE((int)spy->removedConnectionCallbackSlot.size(), 1);

    ID id;
    DisconnectCause cause;
    std::tie(id, cause) = spy->removedConnectionCallbackSlot.front();
    QCOMPARE(id, peer);
    QCOMPARE(cause, DisconnectCause::buyer_invited_with_bad_terms);

    spy->reset();
}

void Test::selling_buyer_requested_invalid_piece() {

    int invalidPieceIndex = 1337;
    ID peer = 0;

    // back to sell mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(), 5);

    // Start session
    firstStart();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    Coin::PublicKey payeeContractPk;
    Coin::RedeemScriptHash payeeFinalScriptHash;
    addBuyerAndGoToReadyForPieceRequest(peer, protocol_wire::BuyerTerms(), protocol_wire::Ready(), payeeContractPk, payeeFinalScriptHash);

    // peer requests invalid piece
    session->processMessageOnConnection(peer, protocol_wire::RequestFullPiece(invalidPieceIndex));

    QVERIFY(spy->onlyCalledRemovedConnection());

    QCOMPARE((int)spy->removedConnectionCallbackSlot.size(), 1);

    ID id;
    DisconnectCause cause;
    std::tie(id, cause) = spy->removedConnectionCallbackSlot.front();
    QCOMPARE(id, peer);
    QCOMPARE(cause, DisconnectCause::buyer_requested_invalid_piece);

    spy->reset();
}

void Test::selling_buyer_interrupted_payment() {

    int pieceIndex = 0;
    ID peer = 0;

    // back to sell mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(), 5);

    // Start session
    firstStart();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    Coin::PublicKey payeeContractPk;
    Coin::RedeemScriptHash payeeFinalScriptHash;
    addBuyerAndGoToReadyForPieceRequest(peer, protocol_wire::BuyerTerms(), protocol_wire::Ready(), payeeContractPk, payeeFinalScriptHash);

    // peer requests valid piece
    receiveValidFullPieceRequest(peer, pieceIndex);

    // peer resends terms, which should cause payment interrupt
    session->processMessageOnConnection(peer, protocol_wire::Buy());

    // check that peer was removed due to this
    assertConnectionRemoved(peer, DisconnectCause::buyer_interrupted_payment);
    spy->reset();
}

void Test::selling_buyer_sent_invalid_payment() {

    int pieceIndex = 0;
    protocol_wire::PieceData data;
    ID peer = 0;

    // Must have valid public in ready message
    Coin::PrivateKey payorContractSk("E9873D79C6D87DC0FB6A5778633389F4");
    protocol_wire::Ready ready = protocol_wire::Ready(1123,
                                                      Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")), 55),
                                                      payorContractSk.toPublicKey(),
                                                      Coin::RedeemScriptHash::fromRawHash(uchar_vector("03a3fac91cac4a5c9ec870b444c4890ec7d68671")));
    // back to sell mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(), 5);

    // Start session
    firstStart();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    Coin::PublicKey payeeContractPk;
    Coin::RedeemScriptHash payeeFinalScriptHash;
    addBuyerAndGoToReadyForPieceRequest(peer, protocol_wire::BuyerTerms(), ready, payeeContractPk, payeeFinalScriptHash);

    // peer requests valid piece
    receiveValidFullPieceRequest(peer, pieceIndex);

    // send message to peer
    sendFullPiece(peer, data, pieceIndex);

    // peer sends invalid payment (blank signature)
    session->processMessageOnConnection(peer, protocol_wire::Payment());

    // check that peer was removed due to this
    assertConnectionRemoved(peer, DisconnectCause::buyer_sent_invalid_payment);
    spy->reset();
}

void Test::selling_buyer_disappears() {

    ID peer = 0;
    uint numberOfExchangesBeforeDisappearance = 5;

    protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);
    protocol_wire::BuyerTerms buyerTerms(24, 200, 2, 400, 5);
    Coin::PrivateKey payorContractSk("E9873D79C6D87DC0FB6A5778633389F4");
    protocol_wire::Ready ready(1123,
                               Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")), 55),
                               payorContractSk.toPublicKey(),
                               Coin::RedeemScriptHash::fromRawHash(uchar_vector("03a3fac91cac4a5c9ec870b444c4890ec7d68671")));

    assert(sellerTerms.satisfiedBy(buyerTerms));

    // back to sell mode
    toSellMode(SellingPolicy(), sellerTerms, 5);

    // Start session
    firstStart();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    Coin::PublicKey payeeContractPk;
    Coin::RedeemScriptHash payeeFinalScriptHash;
    addBuyerAndGoToReadyForPieceRequest(peer, buyerTerms, ready, payeeContractPk, payeeFinalScriptHash);

    // Do sequence of data for payments exchanges
    paymentchannel::Payor payor = getPayor(sellerTerms, ready, payorContractSk, payeeContractPk, payeeFinalScriptHash);

    exchangeDataForPayment(peer, numberOfExchangesBeforeDisappearance, payor);

    // connection is dropped: we can't use Test::removeConnection: does spy->reset, and asserts no other callback
    session->removeConnection(peer);

    // make sure we are notified about claiming last payment
    {
        ID removedPeer;
        joystream::paymentchannel::Payee payee;
        QVERIFY(spy->claimLastPaymentCallbackSlot.size() == 1);

        std::tie(removedPeer, payee) = spy->claimLastPaymentCallbackSlot.front();

        QCOMPARE(removedPeer, peer);
        QCOMPARE((uint)payee.numberOfPaymentsMade(), numberOfExchangesBeforeDisappearance);
    }

    // and that connecion is dropped from session
    assertConnectionRemoved(peer, DisconnectCause::client);
}

void Test::buying_basic() {

    // To buy mode
    BuyingPolicy policy(1000, 1000, protocol_wire::SellerTerms::OrderingPolicy::min_price);
    toBuyMode(Coin::UnspentOutputSet(), policy, protocol_wire::BuyerTerms(), TorrentPieceInformation());

    // Incorrectly try to go to same mode again
    QVERIFY_EXCEPTION_THROWN(toBuyMode(Coin::UnspentOutputSet(), BuyingPolicy(), protocol_wire::BuyerTerms(), TorrentPieceInformation()),
                             exception::SessionAlreadyInThisMode);

    // Start session
    firstStart();

    // incorrectly try to start again
    QVERIFY_EXCEPTION_THROWN(firstStart(), exception::StateIncompatibleOperation);

    // Do basic tests
    basic();

    //// Do transitions to other modes: observe, back to buy, sell

    // Have some new peers join, without announcing mode,
    // and transition to buy mode
    addConnection(0);
    addConnection(1);

    // Go to observe mode
    toObserveMode();

    // Back to buy mode
    toBuyMode(Coin::UnspentOutputSet(), policy, protocol_wire::BuyerTerms(), TorrentPieceInformation());
}

void Test::buying() {

    // min #sellers = 3
    protocol_wire::BuyerTerms buyerTerms(24, 200, 3, 400, 5);

    SellerPeer first(0, protocol_wire::SellerTerms(22, 134, 10, 88, 32), 32),
               second(1, protocol_wire::SellerTerms(4, 13, 11, 88, 32), 13),
               third(2, protocol_wire::SellerTerms(1, 2, 3, 88, 32), 3),
               bad(3, protocol_wire::SellerTerms(), 8);

    assert(buyerTerms.satisfiedBy(first.terms));
    assert(buyerTerms.satisfiedBy(second.terms));
    assert(buyerTerms.satisfiedBy(third.terms));
    assert(!buyerTerms.satisfiedBy(bad.terms));

    // To buy mode
    std::shared_ptr<Coin::UnspentOutput> utxo(
      new Coin::UnspentP2SHOutput(Coin::KeyPair::generate(),
                                  uchar_vector(),
                                  uchar_vector(),
                                  Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                         55),
                                  34561)
    );

    Coin::UnspentOutputSet funding({utxo});

    // minTimeBeforeBuildingContract = 1
    // servicingPieceTimeOutLimit = 2
    BuyingPolicy policy(1, 2, protocol_wire::SellerTerms::OrderingPolicy::min_price);

    uint totalNumberOfPieces = 30;
    TorrentPieceInformation information;
    for(uint i = 0;i < totalNumberOfPieces;i++)
        information.push_back(PieceInformation(0, (i % 2) == 0)); // every even index piece is missing

    toBuyMode(funding, policy, buyerTerms, information);

    // Start session
    firstStart();

    // Add connection and announce seller terms, which are good enough, and hence an invitation should arrive
    add(first);
    assertSellerInvited(first);

    // Add another seller with good terms
    add(second);
    assertSellerInvited(second);

    // Add another seller with bad terms
    add(bad);
    QVERIFY(spy->blank()); // nothing should happen

    // tick()
    session->tick();
    QVERIFY(spy->blank()); // no seller has joined, and there are to few (min 3)

    // Make the two good and one bad join
    session->processMessageOnConnection(first.id, first.setJoiningContract());
    session->processMessageOnConnection(second.id, second.setJoiningContract());
    session->processMessageOnConnection(bad.id, bad.setJoiningContract()); // not legal, should be ignored

    // tick()
    session->tick();
    QVERIFY(spy->blank()); // Nothing should happen, as min 3 good sellers are required

    // Make last good one join
    add(third);
    assertSellerInvited(third);
    spy->reset();

    session->processMessageOnConnection(third.id, third.setJoiningContract());

    // Nothing should happen, due to policy.minTimeBeforeBuildingContract() not having passed
    QVERIFY(spy->blank());

    // wait sufficient amount of time
    std::cout << "Sleeping during testing to trigger buying policy." << std::endl;
    std::this_thread::sleep_for(policy.minTimeBeforeBuildingContract());

    // tick()
    session->tick();

    // make sure contract was announced to only relevant peers
    first.contractAnnounced();
    second.contractAnnounced();
    third.contractAnnounced();
    bad.assertNoContractAnnounced();

    // Verify contract
    QCOMPARE((int)spy->broadcastTransactionCallbackSlot.size(), 1);

    {
        //bool result;
        Coin::Transaction tx;

        std::tie(tx) = spy->broadcastTransactionCallbackSlot.front();

        first.assertContractValidity(tx);
        second.assertContractValidity(tx);
        third.assertContractValidity(tx);
    }

    // asserts for this? requires us to understand
    // _generateKeyPairs(numberOfSellers);
    // _generateP2PKHAddresses(numberOfSellers);

    // Service all piece requests from buyer until done
    do {

        // Attempts to complete a round of exchange for each peer
        completeExchange(first);
        completeExchange(second);
        completeExchange(third);

    } while(hasPendingFullPieceRequest({first, second, third}));

    // assert that state is done, all is good
    // can't access buying sub state!
}

void Test::buying_seller_has_interrupted_contract() {

    // min #sellers = 1
    protocol_wire::BuyerTerms buyerTerms(24, 200, 1, 400, 5);

    SellerPeer first(0, protocol_wire::SellerTerms(22, 134, 10, 88, 32), 32);

    assert(buyerTerms.satisfiedBy(first.terms));

    // To buy mode
    std::shared_ptr<Coin::UnspentOutput> utxo(
      new Coin::UnspentP2SHOutput(Coin::KeyPair::generate(),
                                  uchar_vector(),
                                  uchar_vector(),
                                  Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                         55),
                                  34561)
    );

    Coin::UnspentOutputSet funding({utxo});

    // minTimeBeforeBuildingContract = 0
    // servicingPieceTimeOutLimit = 2
    BuyingPolicy policy(0, 2, protocol_wire::SellerTerms::OrderingPolicy::min_price);

    uint totalNumberOfPieces = 30;
    TorrentPieceInformation information;
    for(uint i = 0;i < totalNumberOfPieces;i++)
        information.push_back(PieceInformation(0, (i % 2) == 0)); // every even index piece is missing

    ////

    toBuyMode(funding, policy, buyerTerms, information);

    // Start session
    firstStart();

    // Take seller to exchange
    takeSingleSellerToExchange(first);

    // Service all piece requests from buyer until done
    completeExchange(first);

    QVERIFY(hasPendingFullPieceRequest({first}));

    spy->reset();

    // Peer interrupts contract by resending terms
    session->processMessageOnConnection(first.id, protocol_wire::Buy());

    // assert removal
    assertConnectionRemoved(first.id, DisconnectCause::seller_has_interrupted_contract);

    // extra check that no other callback was made
    QVERIFY(spy->onlyCalledRemovedConnection());

    spy->reset();
}

void Test::buying_seller_servicing_piece_has_timed_out() {

    // min #sellers = 1
    protocol_wire::BuyerTerms buyerTerms(24, 200, 1, 400, 5);

    SellerPeer first(0, protocol_wire::SellerTerms(22, 134, 10, 88, 32), 32);

    assert(buyerTerms.satisfiedBy(first.terms));

    // To buy mode
    std::shared_ptr<Coin::UnspentOutput> utxo(
      new Coin::UnspentP2SHOutput(Coin::KeyPair::generate(),
                                  uchar_vector(),
                                  uchar_vector(),
                                  Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                         55),
                                  34561)
    );

    Coin::UnspentOutputSet funding({utxo});

    // minTimeBeforeBuildingContract = 0
    // servicingPieceTimeOutLimit = 2
    BuyingPolicy policy(0, 2, protocol_wire::SellerTerms::OrderingPolicy::min_price);

    uint totalNumberOfPieces = 30;
    TorrentPieceInformation information;
    for(uint i = 0;i < totalNumberOfPieces;i++)
        information.push_back(PieceInformation(0, (i % 2) == 0)); // every even index piece is missing

    ////

    toBuyMode(funding, policy, buyerTerms, information);

    // Start session
    firstStart();

    // Take seller to exchange
    takeSingleSellerToExchange(first);

    spy->reset();

    // wait sufficient amount of time
    std::cout << "Sleeping to have piece request to time out." << std::endl;
    std::this_thread::sleep_for(policy.servicingPieceTimeOutLimit());

    // Processes expiry
    session->tick();

    // assert removal
    assertConnectionRemoved(first.id, DisconnectCause::seller_servicing_piece_has_timed_out);

    // extra check that no other callback was made
    QVERIFY(spy->onlyCalledRemovedConnection());

    spy->reset();
}

void Test::buying_seller_sent_invalid_piece() {

    // min #sellers = 1
    protocol_wire::BuyerTerms buyerTerms(24, 200, 1, 400, 5);

    SellerPeer first(0, protocol_wire::SellerTerms(22, 134, 10, 88, 32), 32);

    assert(buyerTerms.satisfiedBy(first.terms));

    // To buy mode
    std::shared_ptr<Coin::UnspentOutput> utxo(
      new Coin::UnspentP2SHOutput(Coin::KeyPair::generate(),
                                  uchar_vector(),
                                  uchar_vector(),
                                  Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                         55),
                                  34561)
    );

    Coin::UnspentOutputSet funding({utxo});

    // minTimeBeforeBuildingContract = 0
    // servicingPieceTimeOutLimit = 2
    BuyingPolicy policy(0, 2, protocol_wire::SellerTerms::OrderingPolicy::min_price);

    uint totalNumberOfPieces = 30;
    TorrentPieceInformation information;
    for(uint i = 0;i < totalNumberOfPieces;i++)
        information.push_back(PieceInformation(0, (i % 2) == 0)); // every even index piece is missing

    ////

    toBuyMode(funding, policy, buyerTerms, information);

    // Start session
    firstStart();

    // Take seller to exchange
    takeSingleSellerToExchange(first);

    // Recover index of requested piece
    int requestedPiece;
    {
        ConnectionSpy<ID> * c = first.spy;
        QCOMPARE((int)c->sendMessageOnConnectionCallbackSlot.size(), 1);
        const protocol_wire::ExtendedMessagePayload * m;
        std::tie(m) = c->sendMessageOnConnectionCallbackSlot.front();

        QCOMPARE(m->messageType(), protocol_wire::MessageType::request_full_piece);
        const protocol_wire::RequestFullPiece * m2 = dynamic_cast<const protocol_wire::RequestFullPiece *>(m);

        requestedPiece = m2->pieceIndex();
    }

    // Send piece
    session->processMessageOnConnection(first.id, protocol_wire::FullPiece());

    spy->reset();

    //
    session->invalidPieceReceivedOnConnection(first.id, requestedPiece);

    // assert removal
    assertConnectionRemoved(first.id, DisconnectCause::seller_sent_invalid_piece);

    // extra check that no other callback was made
    QVERIFY(spy->onlyCalledRemovedConnection());

    spy->reset();
}

Coin::PrivateKey Test::nextPrivateKey() {

    std::stringstream s;
    s << std::hex << nextKey++;
    std::string hexInteger = s.str(); // MSB is at index 0

    // Make even length representation by potentially adding leading 0
    if(hexInteger.length() % 2 != 0)
        hexInteger = "0" + hexInteger;

    std::string finalHexRepresentation;

    if(hexInteger.length() > 2*Coin::Seed::length())
        throw std::runtime_error("privateKeyFromUInt: argument to big"); // not even going to truncate
    else if(hexInteger.length() < 2*Coin::Seed::length())
        finalHexRepresentation = std::string(2*Coin::Seed::length() - hexInteger.length(), '0') + hexInteger; // add suitable number of leading 0s

    assert(finalHexRepresentation.length() == 2*Coin::Seed::length());

    //std::cout << "Generate ---- " << finalHexRepresentation.length() << std::endl;

    Coin::Seed seed(finalHexRepresentation.c_str());

    Coin::HDKeychain keyChain(seed.generateHDKeychain());

    return Coin::PrivateKey(keyChain.privkey());
}

paymentchannel::Payor Test::getPayor(const protocol_wire::SellerTerms & sellerTerms,
                                     const protocol_wire::Ready & ready,
                                     const Coin::PrivateKey & payorContractSk,
                                     const Coin::PublicKey & payeeContractPk,
                                     const Coin::PubKeyHash & payeeFinalPkHash) {

   return paymentchannel::Payor(sellerTerms.minPrice(),
                                0,
                                ready.value(),
                                sellerTerms.settlementFee(),
                                Coin::RelativeLockTime::fromTimeUnits(sellerTerms.minLock()),
                                ready.anchor(),
                                Coin::KeyPair(payorContractSk),
                                ready.finalPkHash(),
                                payeeContractPk,
                                payeeFinalPkHash);



}

////

void Test::basic() {

    //// (1) four peers join without announcing their mode

    addConnection(0);
    addConnection(1);
    addConnection(2);
    addConnection(3);

    // add same as before
    QVERIFY_EXCEPTION_THROWN(addConnection(0), joystream::protocol_session::exception::ConnectionAlreadyAddedException<ID>);

    //// (2) one drops out

    removeConnection(3);

    //// (3) the rest change modes

    session->processMessageOnConnection(0, protocol_wire::Observe());
    session->processMessageOnConnection(1, protocol_wire::Sell(protocol_wire::SellerTerms(1,2,3,4,5), 1337));
    session->processMessageOnConnection(2, protocol_wire::Buy(protocol_wire::BuyerTerms(10,20,30,40,50)));

    // Reset in case it is a buying session, where above terms warrant invite
    spy->reset();

    //// (4) pause

    pause();

    //// (5) new peer joins

    addConnection(4);

    //// (6) old peer updates terms

    session->processMessageOnConnection(0, protocol_wire::Observe());
    session->processMessageOnConnection(1, protocol_wire::Sell(protocol_wire::SellerTerms(33,44,66,22,32), 1338));
    session->processMessageOnConnection(2, protocol_wire::Buy(protocol_wire::BuyerTerms(100,200,300,400,500)));

    //// (7) stop

    stop();

    //// (8) start

    firstStart();
}

void Test::addConnection(ID id) {

    ConnectionSpy<ID> * c = spy->addConnection(id);

    assertTermsSentToPeer(c->sendMessageOnConnectionCallbackSlot);

    // Check the state of session
    QVERIFY(session->hasConnection(id));

    auto ids = session->connectionIds();
    auto itr = std::find(ids.cbegin(), ids.cend(), id);
    QVERIFY(itr != ids.cend());

    QVERIFY(spy->blankSession());

    spy->reset();
}

void Test::removeConnection(ID id) {

    // remove peer
    session->removeConnection(id);

    // assert removal
    assertConnectionRemoved(id, DisconnectCause::client);

    // extra check that no other callback was made
    QVERIFY(spy->onlyCalledRemovedConnection());

    spy->reset();
}

void Test::firstStart() {

    session->start();

    QCOMPARE(session->state(), SessionState::started);
    QVERIFY(spy->blankSession());

    // iterate peers and check that correct mode message had been sent to whatever
    for(auto mapping : spy->connectionSpies)
        assertTermsSentToPeer(mapping.second->sendMessageOnConnectionCallbackSlot);

    spy->reset();
}

void Test::stop() {

    SessionMode mode = session->mode();
    assert(mode != SessionMode::not_set);

    // Get all connections present before stop
    std::set<ID> presentBeforeStop = session->connectionIds();

    session->stop();

    QCOMPARE(session->state(), SessionState::stopped);

    // Check that removal callback was made for each
    std::set<ID> removed;
    for(auto f : spy->removedConnectionCallbackSlot)
        removed.insert(std::get<0>(f));

    QCOMPARE(removed, presentBeforeStop);

    spy->reset();
    spy->removeConnectionSpies();
}

void Test::pause() {

    session->pause();
    QCOMPARE(session->state(), SessionState::paused);
    QVERIFY(spy->blank());
}

void Test::toObserveMode() {

    spy->toMonitoredObserveMode();

    QCOMPARE(session->mode(), SessionMode::observing);
}

void Test::toSellMode(const SellingPolicy & policy,
                      const protocol_wire::SellerTerms & terms,
                      int maxPieceIndex) {

    spy->toMonitoredSellMode(policy, terms, maxPieceIndex);

    QCOMPARE(session->mode(), SessionMode::selling);
}

void Test::toBuyMode(const Coin::UnspentOutputSet & funding,
                     const BuyingPolicy & policy,
                     const protocol_wire::BuyerTerms & terms,
                     const TorrentPieceInformation & information) {

    spy->toMonitoredBuyMode(funding,
                           policy,
                           terms,
                           information);

    QCOMPARE(session->mode(), SessionMode::buying);
}

void Test::assertConnectionRemoved(ID expectedId, DisconnectCause expectedCause) const {

    // peer is gone
    QVERIFY(!session->hasConnection(expectedId));
    auto ids = session->connectionIds();
    auto itr = std::find(ids.cbegin(), ids.cend(), expectedId);
    QVERIFY(itr == ids.cend());

    // Iterate removal callbacks, in case multiple peers are removed at ones (e.g. stopping):
    // exactly one removal callback was made for given peer,
    // and with given cause
    bool callbackHasBeenMade = false;
    for(auto f : spy->removedConnectionCallbackSlot) {

        ID removedPeer;
        DisconnectCause cause;
        std::tie(removedPeer, cause) = f;

        // is this the given peer
        if(removedPeer == expectedId) {

            // is this correct cause
            if(cause == expectedCause) {

                if(callbackHasBeenMade)
                    QFAIL("correct removedConnection callback made multiple times.");
                else
                    callbackHasBeenMade = true;

            } else
               QFAIL("removedConnection callback made for peer with incorrect cause.");
        }


    }

    if(!callbackHasBeenMade)
        QFAIL("No removedConnection callback made for peer.");

    // notice that we dont clear spy
}

void Test::assertTermsSentToPeer(const SendMessageOnConnectionCallbackSlot & slot) const {

    QCOMPARE((int)slot.size(), 1);
    auto f = slot.front();
    const protocol_wire::ExtendedMessagePayload * m = std::get<0>(f);

    // having a hard time comparing terms, as they cannot be recovered
    // from session easily

    switch(session->mode()) {
        case SessionMode::not_set: assert(false);
        case SessionMode::buying:
        QCOMPARE(m->messageType(), protocol_wire::MessageType::buy);
        //QCOMPARE(session->)

        break;
        case SessionMode::selling:
        QCOMPARE(m->messageType(), protocol_wire::MessageType::sell);

        break;
        case SessionMode::observing:
        QCOMPARE(m->messageType(), protocol_wire::MessageType::observe);
        break;
        default:
            assert(false);
    }
}

void Test::assertTermsSentToAllPeers() const {

    for(auto mapping : spy->connectionSpies)
        assertTermsSentToPeer(mapping.second->sendMessageOnConnectionCallbackSlot);
}

void Test::assertFullPieceSent(ID peer, const protocol_wire::PieceData & data) const {

    ConnectionSpy<ID> * c = spy->connectionSpies.at(peer);

    QCOMPARE((int)c->sendMessageOnConnectionCallbackSlot.size(), 1);
    const protocol_wire::ExtendedMessagePayload * m;
    std::tie(m) = c->sendMessageOnConnectionCallbackSlot.front();

    QCOMPARE(m->messageType(), protocol_wire::MessageType::full_piece);
    const protocol_wire::FullPiece * m2 = dynamic_cast<const protocol_wire::FullPiece *>(m);

    QCOMPARE(m2->pieceData(), data);
}

////

void Test::addBuyerAndGoToReadyForPieceRequest(ID id, const protocol_wire::BuyerTerms & terms, const protocol_wire::Ready & ready, Coin::PublicKey & payeeContractPk, Coin::PubKeyHash & payeeFinalPkHash) {

    // peer joins
    addConnection(id);

    // peer announces buyer mode
    session->processMessageOnConnection(id, protocol_wire::Buy(terms));

    QVERIFY(spy->blank());

    // peer (buyer) sends invitation to terms which are good enough
    // NB: we assume the seller has only sent one terms message, which has 0 index
    session->processMessageOnConnection(id, protocol_wire::JoinContract(0));

    // key pair was generated
    QCOMPARE((int)spy->generateKeyPairCallbackSlot.size(), 1);

    // address was generated
    QCOMPARE((int)spy->generateReceiveAddressesCallbackSlot.size(), 1);
    QCOMPARE((int)std::get<0>(spy->generateReceiveAddressesCallbackSlot.front()), 1);

    // client joined contract
    ConnectionSpy<ID> * cSpy = spy->connectionSpies.at(id);

    QCOMPARE((int)cSpy->sendMessageOnConnectionCallbackSlot.size(), 1);
    const protocol_wire::ExtendedMessagePayload * m;
    std::tie(m) = cSpy->sendMessageOnConnectionCallbackSlot.front();

    QCOMPARE(m->messageType(), protocol_wire::MessageType::joining_contract);
    const protocol_wire::JoiningContract * m2 = dynamic_cast<const protocol_wire::JoiningContract *>(m);

    // copy out return values which payee sent
    payeeContractPk = m2->contractPk();
    payeeFinalPkHash = m2->finalPkHash();

    spy->reset();

    // peer (buyer) announces ready
    session->processMessageOnConnection(id, ready);

    // check that announcement callback was made
    QVERIFY(spy->onlyCalledAnchorAnnounced());
    QCOMPARE((int)spy->anchorAnnouncedCallbackSlot.size(), 1);

    ID announcedId;
    quint64 value;
    Coin::typesafeOutPoint anchor;
    Coin::PublicKey contractPk;
    Coin::PubKeyHash finalPkHash;

    std::tie(announcedId, value, anchor, contractPk, finalPkHash) = spy->anchorAnnouncedCallbackSlot.front();

    QCOMPARE(announcedId, id);
    QCOMPARE(value, ready.value());
    QCOMPARE(anchor, ready.anchor());
    QCOMPARE(contractPk, ready.contractPk());
    QCOMPARE(finalPkHash, ready.finalPkHash());

    spy->reset();
}

void Test::receiveValidFullPieceRequest(ID peer, int pieceIndexToRequest) {

    session->processMessageOnConnection(peer, protocol_wire::RequestFullPiece(pieceIndexToRequest));

    QVERIFY(spy->onlyCalledLoadPieceForBuyer());
    QCOMPARE((int)spy->loadPieceForBuyerCallbackSlot.size(), 1);

    ID id;
    int pieceIndexToLoad;

    std::tie(id, pieceIndexToLoad) = spy->loadPieceForBuyerCallbackSlot.front();
    QCOMPARE(id, peer);
    QCOMPARE(pieceIndexToRequest, pieceIndexToLoad);

    spy->reset();
}

void Test::sendFullPiece(ID id, const protocol_wire::PieceData & data, int pieceIndex) {

    session->pieceLoaded(id, data, pieceIndex);

    QVERIFY(spy->blankSession());

    ConnectionSpy<ID> * c = spy->connectionSpies.at(id);

    QCOMPARE((int)c->sendMessageOnConnectionCallbackSlot.size(), 1);
    const protocol_wire::ExtendedMessagePayload * m;
    std::tie(m) = c->sendMessageOnConnectionCallbackSlot.front();

    QCOMPARE(m->messageType(), protocol_wire::MessageType::full_piece);
    const protocol_wire::FullPiece * m2 = dynamic_cast<const protocol_wire::FullPiece *>(m);

    QCOMPARE(m2->pieceData(), data);

    spy->reset();
}

void Test::exchangeDataForPayment(ID peer, uint N, paymentchannel::Payor & payor) {

    for(uint i = 0;i < N;i++) {

        // peer requests valid piece
        receiveValidFullPieceRequest(peer, i);

        // send message to peer
        protocol_wire::PieceData data;
        sendFullPiece(peer, data, i);

        // generate new payment
        Coin::Signature sig = payor.makePayment();

        // peer sends valid payment
        session->processMessageOnConnection(peer, protocol_wire::Payment(sig));

        QVERIFY(spy->blank());
    }

}

void Test::add(SellerPeer & peer) {

    addConnection(peer.id);
    peer.spy = spy->connectionSpies.at(peer.id);
    session->processMessageOnConnection(peer.id, protocol_wire::Sell(peer.terms, peer.index));
}

void Test::completeExchange(SellerPeer & peer) {

    ConnectionSpy<ID> * c = peer.spy;

    // is there a bending full piece request?
    if(c->sendMessageOnConnectionCallbackSlot.size() == 0)
        return;

    int requestedPiece = 0;

    {
        QCOMPARE((int)c->sendMessageOnConnectionCallbackSlot.size(), 1);
        const protocol_wire::ExtendedMessagePayload * m;
        std::tie(m) = c->sendMessageOnConnectionCallbackSlot.front();

        QCOMPARE(m->messageType(), protocol_wire::MessageType::request_full_piece);
        const protocol_wire::RequestFullPiece * m2 = dynamic_cast<const protocol_wire::RequestFullPiece *>(m);

        requestedPiece = m2->pieceIndex();

        // Remove message from slot
        c->sendMessageOnConnectionCallbackSlot.clear();
    }

    // if so respond with some data
    protocol_wire::PieceData dataSent = protocol_wire::PieceData::fromHex("179017230471923470");
    session->processMessageOnConnection(peer.id, protocol_wire::FullPiece(dataSent));

    // check that session made client notification about arrival of piece
    QCOMPARE((int)spy->fullPieceArrivedCallbackSlot.size(), 1);

    {
        ID id;
        protocol_wire::PieceData receivedData;
        int index;
        std::tie(id, receivedData, index) = spy->fullPieceArrivedCallbackSlot.front();

        QCOMPARE(id, peer.id);
        QCOMPARE(receivedData, dataSent);
        QCOMPARE(index, requestedPiece);

        // tell session that it was valid
        session->validPieceReceivedOnConnection(peer.id, index);

        // clear call
        spy->fullPieceArrivedCallbackSlot.clear();
    }

    // check that a payment was sent to the peer
    // message should be: payment, full_piece_request
    {
        QVERIFY((int)c->sendMessageOnConnectionCallbackSlot.size() > 0);
        const protocol_wire::ExtendedMessagePayload * m;
        std::tie(m) = c->sendMessageOnConnectionCallbackSlot.front();

        QCOMPARE(m->messageType(), protocol_wire::MessageType::payment);
        const protocol_wire::Payment * m2 = dynamic_cast<const protocol_wire::Payment *>(m);

        // validate payment
        peer.validatePayment(m2->sig());

        // Ditch payment message
        delete m2;
        c->sendMessageOnConnectionCallbackSlot.pop_front();
    }
}

bool Test::hasPendingFullPieceRequest(const std::vector<SellerPeer> & peers) {

    bool has = false;

    for(auto p : peers)
        has = has || p.hasPendingFullPieceRequest();

    return has;
}

void Test::takeSingleSellerToExchange(SellerPeer & peer) {

    // Add connection and announce seller terms
    add(peer);
    assertSellerInvited(peer);
    spy->reset();

    // Have peer join
    session->processMessageOnConnection(peer.id, peer.setJoiningContract());

    // no need to wait due to policy

    // tick()
    session->tick();

    // make sure contract was announced to only relevant peers
    peer.contractAnnounced();

    // Verify contract
    QCOMPARE((int)spy->broadcastTransactionCallbackSlot.size(), 1);
    Coin::Transaction tx = std::get<0>(spy->broadcastTransactionCallbackSlot.front());

    peer.assertContractValidity(tx);

    // asserts for this? requires us to understand
    // _generateKeyPairs(numberOfSellers);
    // _generateP2PKHAddresses(numberOfSellers);
}

/**
void Test::join(const SellerPeer & peer) {
    session->processMessageOnConnection(peer.id, peer.joinMessage());
}
*/

void Test::assertSellerInvited(const SellerPeer & peer) {

    // client joined contract
    QCOMPARE((int)peer.spy->sendMessageOnConnectionCallbackSlot.size(), 1);
    const protocol_wire::ExtendedMessagePayload * m;
    std::tie(m) = peer.spy->sendMessageOnConnectionCallbackSlot.front();

    QCOMPARE(m->messageType(), protocol_wire::MessageType::join_contract);
    const protocol_wire::JoinContract * m2 = dynamic_cast<const protocol_wire::JoinContract *>(m);

    QCOMPARE(m2->index(), peer.index);

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
