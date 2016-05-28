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

void Test::selling() {

    // same as general stuff as above

    //

    //void toSellMode(Session<ID> * session, SessionSpy<ID> * spy);


    // pausing a seller?
    // if there is a pending request, then make an additional call
    // on session to load piece. This should result in a final full piece being sent???
    // NB: ****this makes no sense, as connection will be gone.***
    // no case for: mode == SessionMode::observing
    // *** make sure that you deal with this later


    // update terms

    /**
    LoadPieceForBuyerCallbackSlot<ConnectionIdType> loadPieceForBuyerCallbackSlot;
    ClaimLastPaymentCallbackSlot<ConnectionIdType> claimLastPaymentCallbackSlot;
    AnchorAnnouncedCallbackSlot<ConnectionIdType> anchorAnnouncedCallbackSlot;

    //// Selling

    // Data for given piece has been loaded
    void pieceLoaded(const ConnectionIdType &, const protocol_wire::PieceData &, int);

    // Update terms when selling
    void updateTerms(const protocol_wire::SellerTerms &);
    */
}

void Test::buying() {

    //void toBuyMode(Session<ID> * session, SessionSpy<ID> * spy);

    // tick(): right away, and see that invites are sent

    // have some peers send
    // tick(): check that invites are not sent to peers


    /**

      check
    BroadcastTransactionCallbackSlot broadcastTransactionCallbackSlot;
    FullPieceArrivedCallbackSlot<ConnectionIdType> fullPieceArrivedCallbackSlot;


    //// Buying

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
    QCOMPARE((int)spy->removedConnectionCallbackSlot.size(), 1);

    auto f = spy->removedConnectionCallbackSlot.front();
    QCOMPARE(std::get<0>(f), id);
    QCOMPARE(std::get<1>(f), DisconnectCause::client);

    spy->reset();
}

void Test::start() {

    session->start();

    QCOMPARE(session->state(), SessionState::started);

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

Coin::PrivateKey Test::privateKeyFromUInt(uint i) {

    std::stringstream s;
    s << std::hex << i;
    std::string hexInteger = s.str();
    return Coin::PrivateKey(hexInteger);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
