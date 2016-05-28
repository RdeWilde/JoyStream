/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#include <Test.hpp>

template<>
std::string IdToString<ID>(const ID & s) {
    return std::to_string(s);
}

Test::Test()
    : network(Coin::Network::testnet3)
    , session(nullptr)
    , spy(nullptr) {
}

void Test::init() {

    session = new Session<ID>();

    spy = new
    SessionSpy<ID>(
    [this](int n) -> std::vector<Coin::KeyPair> {

        std::vector<Coin::KeyPair> keys;

        for(int i = 0;i < n;i++)
            keys.push_back(Coin::KeyPair(privateKeyFromUInt(i)));

        return keys;
    },
    [this](int n) {

        std::vector<Coin::P2PKHAddress> addresses;

        for(int i = 0;i < n;i++)
            addresses.push_back((privateKeyFromUInt(i)).toPublicKey().toP2PKHAddress(this->network));

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
    start();

    // incorrectly try to start again
    QVERIFY_EXCEPTION_THROWN(start(), exception::StateIncompatibleOperation);

    // Do basic tests
    basic();

    //// Do transitions to other modes: buy, back to observe, sell

    // Have some new peers join, without announcing mode,
    // and transition to buy mode
    addConnection(0);
    addConnection(1);

    // Go to buy mode
    toBuyMode(Coin::UnspentP2PKHOutput(),
              BuyingPolicy(),
              protocol_wire::BuyerTerms(),
              TorrentPieceInformation());

    // back to observe mode
    toObserveMode();

    // Go to sell mode
    toSellMode(SellingPolicy(),
               protocol_wire::SellerTerms());

}

void Test::selling_basic() {

    // to observe mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(22, 134, 1, 88, 32));

    // incorrectly try to go to same mode again
    QVERIFY_EXCEPTION_THROWN(toSellMode(SellingPolicy(), protocol_wire::SellerTerms()),
                             exception::SessionAlreadyInThisMode);

    // Start session
    start();

    // incorrectly try to start again
    QVERIFY_EXCEPTION_THROWN(start(), exception::StateIncompatibleOperation);

    // Do basic tests
    basic();

    //// Do transitions to other modes: buy, back to sell, observe

    // Have some new peers join, without announcing mode,
    // and transition to buy mode
    addConnection(0);
    addConnection(1);

    // Go to buy mode
    toBuyMode(Coin::UnspentP2PKHOutput(),
              BuyingPolicy(),
              protocol_wire::BuyerTerms(),
              TorrentPieceInformation());

    // back to sell mode
    toSellMode(SellingPolicy(), protocol_wire::SellerTerms(22, 134, 1, 88, 32));

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
                               Coin::PubKeyHash("03a3fac91cac4a5c9ec870b444c4890ec7d68671"));

    ID peer = 0;

    assert(sellerTerms.satisfiedBy(buyerTerms));

    // back to sell mode
    toSellMode(SellingPolicy(), sellerTerms);

    // Start session
    start();

    // Add a buyer peer and take connection to state ReadyForPieceRequest
    addBuyerAndGoToReadyForPieceRequest(peer, buyerTerms, ready);


    // Repeat N times: i)
        // peer (buyer) requests a piece
        // assert ---> seller session asks // loadPieceForBuyerCallbackSlot.hook(),

        // Request invalid piece!! disconnect

        // session-*pieceLoaded(const ConnectionIdType &, const protocol_wire::PieceData &, int);
        // assert ---> sessino sends the correct message on the given wire

    // N+1)
    // peer (buyer dis appears)
    // assert that seller session calls: claimLastPaymentCallbackSlot.hook(),
    // and that it has correct state (#payments, sigs, etc).

    //-----

    // Update terms when selling
    // void updateTerms(const protocol_wire::SellerTerms &);

    // pause/stop different places here
    // stop different places here
    // peer updates its terms
    // peer drops out

    // pausing a seller?
    // if there is a pending request, then make an additional call
    // on session to load piece. This should result in a final full piece being sent???
    // NB: ****this makes no sense, as connection will be gone.***
    // no case for: mode == SessionMode::observing
    // *** make sure that you deal with this later
}

void Test::selling_buyer_invited_with_bad_terms() {

    protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);
    protocol_wire::BuyerTerms badBuyerTerms;
    ID peer = 0;

    assert(!sellerTerms.satisfiedBy(badBuyerTerms));

    // back to sell mode
    toSellMode(SellingPolicy(), sellerTerms);

    // Start session
    start();

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

}

void Test::selling_buyer_interrupted_payment() {

}

void Test::selling_buyer_sent_invalid_payment() {

}

void Test::buying_basic() {

}

void Test::buying() {

    //void toBuyMode(Session<ID> * session, SessionSpy<ID> * spy);

    // tick(): right away, and see that invites are sent

    // have some peers send
    // tick(): check that invites are not sent to peers


    /**

    BroadcastTransactionCallbackSlot broadcastTransactionCallbackSlot;
    FullPieceArrivedCallbackSlot<ConnectionIdType> fullPieceArrivedCallbackSlot;

    // A valid piece was sent too us on given connection
    void validPieceReceivedOnConnection(const ConnectionIdType &, int index);

    // An invalid piece was sent too us on given connection
    void invalidPieceReceivedOnConnection(const ConnectionIdType &, int index);

    // Piece with given index has been downloaded, but not through
    // a regitered connection. Could be non-joystream peers, or something out of bounds.
    void pieceDownloaded(int);

    // Update terms
    void updateTerms(const protocol_wire::BuyerTerms &);

    */

}

void Test::buying_seller_has_interrupted_contract() {
//DisconnectCause
}

void Test::buying_seller_servicing_piece_has_timed_out() {
//DisconnectCause
}

void Test::buying_seller_sent_invalid_piece() {
//DisconnectCause
}

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

    start();
}

void Test::addConnection(ID id) {

    ConnectionSpy<ID> * c = spy->addConnection(id);

    verifyTermsSentToPeer(c->sendMessageOnConnectionCallbackSlot);

    // Check the state of session
    QVERIFY(session->hasConnection(id));

    auto ids = session->connectionIds();
    auto itr = std::find(ids.cbegin(), ids.cend(), id);
    QVERIFY(itr != ids.cend());

    QVERIFY(spy->blankSession());

    spy->reset();
}

void Test::removeConnection(ID id) {

    session->removeConnection(id);

    QVERIFY(!session->hasConnection(id));
    auto ids = session->connectionIds();
    auto itr = std::find(ids.cbegin(), ids.cend(), id);
    QVERIFY(itr == ids.cend());

    QVERIFY(spy->onlyCalledRemovedConnection());

    ID removedPeer;
    DisconnectCause cause;
    std::tie(removedPeer, cause) = spy->removedConnectionCallbackSlot.front();
    QCOMPARE((int)spy->removedConnectionCallbackSlot.size(), 1);
    QCOMPARE(removedPeer, id);
    QCOMPARE(cause, DisconnectCause::client);

    spy->reset();
}

void Test::start() {

    session->start();

    QCOMPARE(session->state(), SessionState::started);
    QVERIFY(spy->blankSession());

    // iterate peers and check that correct mode message had been sent to whatever
    for(auto mapping : spy->connectionSpies)
        verifyTermsSentToPeer(mapping.second->sendMessageOnConnectionCallbackSlot);

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

void Test::addBuyerAndGoToReadyForPieceRequest(ID id, const protocol_wire::BuyerTerms & terms, const protocol_wire::Ready & ready) {

    // peer joins
    addConnection(id);

    // peer announces buyer mode
    session->processMessageOnConnection(id, protocol_wire::Buy(terms));

    QVERIFY(spy->blank());

    // peer (buyer) sends invitation to terms which are good enough
    // NB: we assume the seller has only sent one terms message, which has 0 index
    session->processMessageOnConnection(id, protocol_wire::JoinContract(0));

    // key pair was generated
    QCOMPARE((int)spy->generateKeyPairsCallbackSlot.size(), 1);
    QCOMPARE((int)std::get<0>(spy->generateKeyPairsCallbackSlot.front()), 1);

    // address was generated
    QCOMPARE((int)spy->generateP2PKHAddressesCallbackSlot.size(), 1);
    QCOMPARE((int)std::get<0>(spy->generateP2PKHAddressesCallbackSlot.front()), 1);

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

void Test::toObserveMode() {

    spy->toMonitoredObserveMode();

    QCOMPARE(session->mode(), SessionMode::observing);
}

void Test::toSellMode(const SellingPolicy & policy,
                      const protocol_wire::SellerTerms & terms) {

    spy->toMonitoredSellMode(policy, terms);

    QCOMPARE(session->mode(), SessionMode::selling);
}

void Test::toBuyMode(const Coin::UnspentP2PKHOutput & funding,
                     const BuyingPolicy & policy,
                     const protocol_wire::BuyerTerms & terms,
                     const TorrentPieceInformation & information) {

    spy->toMonitoredBuyMode(funding,
                           policy,
                           terms,
                           information);

    QCOMPARE(session->mode(), SessionMode::buying);


}

void Test::verifyTermsSentToPeer(const SendMessageOnConnectionCallbackSlot & slot) {

    QCOMPARE((int)slot.size(), 1);
    auto f = slot.front();
    const protocol_wire::ExtendedMessagePayload * m = std::get<0>(f);

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

Coin::PrivateKey Test::privateKeyFromUInt(uint) {

    /**
     * For whatever reason, the private keys generated
     * by this routine seem to be invalid? the EC routines fail to convert
     * them properly to public keys.

    std::stringstream s;
    s << std::hex << i;
    std::string hexInteger = s.str(); // MSB is at index 0

    // Make even length representation by potentially adding leading 0
    if(hexInteger.length() % 2 != 0)
        hexInteger = "0" + hexInteger;

    std::string finalHexRepresentation;

    if(hexInteger.length() > 2*Coin::PrivateKey::length())
        throw std::runtime_error("privateKeyFromUInt: argument to big"); // not even going to truncate
    else if(hexInteger.length() < 2*Coin::PrivateKey::length())
        finalHexRepresentation = std::string(2*Coin::PrivateKey::length() - hexInteger.length(), '0') + hexInteger; // add suitable number of leading 0s

    assert(finalHexRepresentation.length() == 2*Coin::PrivateKey::length());

    std::cout << "Generate ---- " << finalHexRepresentation.length() << std::endl;

    return Coin::PrivateKey(finalHexRepresentation);
    */

    return Coin::PrivateKey::generate();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
