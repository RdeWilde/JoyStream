/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#include <Test.hpp>
#include <SessionSpy.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

typedef uint ID;

template<>
std::string IdToString<ID>(const ID & s) {
    return std::to_string(s);
}

void Test::session() {

    Coin::Network network = Coin::Network::testnet3;

    SessionSpy<ID> spy(
    [this](int n) -> std::vector<Coin::KeyPair> {

        std::vector<Coin::KeyPair> keys;

        for(int i = 0;i < n;i++)
            keys.push_back(Coin::KeyPair::generate());

        return keys;
    },
    [this, &network](int n) {

        std::vector<Coin::P2PKHAddress> addresses;

        for(int i = 0;i < n;i++)
            addresses.push_back(Coin::PrivateKey::generate().toPublicKey().toP2PKHAddress(network));

        return addresses;

    },
    [this](const Coin::Transaction &) {
        return true;
    });

    Session<ID> session;

    // Start as observer
    session.toObserveMode();

    // Add connection which is monitored
    ID c0 = 0;
    ConnectionSpy<ID> connectionSpy(c0);
    session.addConnection(c0, connectionSpy.sendMessageOnConnectionCallbackSlot.hook());

    session.start();

    // Verify that mode message sent to peer
    QVERIFY(connectionSpy.sendMessageOnConnectionCallbackSlot.called);
    connectionSpy.reset();

    // but nothing has otherwised happned in the session
    QVERIFY(spy.blank());
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
