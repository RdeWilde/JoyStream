/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

#include <SessionSpy.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

// Id type used to identify connections
typedef uint ID;

class Test : public QObject {

    Q_OBJECT

public:

    Test();

private slots:

    // Runs before each unit, sets up session and spy

    void initTestCase();
    void cleanup();

    //// Cases

    //
    void observing();

    //
    void selling();

    //
    void buying();

private:

    // Variable shared across all units tests

    Coin::Network network;
    Session<ID> * session;
    SessionSpy<ID> * spy;

    // Generators for predictable keys & addresse &...

    //// Routines for doing spesific set of tests which can be used across number of cases
    //// NB: None of these routines can return values, as they use QTest macroes which dont return this value.
    //// NB: Spy is always reset, if affected, by each call

    // Takes started session and spy for session:
    // (1) four peers join without announcing their mode
    // (2) one drops out
    // (3) the rest change modes
    // (4) pause
    // (5) new peer joins
    // (6) old peer updates terms
    // (7) stop
    void basic();

    // Adds peer
    void addConnection(ID);

    // Peer disconnects
    void removeConnection(ID);

    // Alter session state
    void start();

    //
    void stop();

    //
    void pause();

    // Alter session mode
    void toObserveMode();

    void toSellMode(const SellingPolicy &,
                    const protocol_wire::SellerTerms &);

    void toBuyMode(const Coin::UnspentP2PKHOutput &,
                   const BuyingPolicy &,
                   const protocol_wire::BuyerTerms &,
                   const TorrentPieceInformation &);

    // Utilities
    void verifyTermsSentToPeer(const SendMessageOnConnectionCallbackSlot &);
};

#endif // TEST_HPP
