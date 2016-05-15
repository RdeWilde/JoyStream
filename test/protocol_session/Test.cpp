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

template<>
std::string IdToString<std::string>(const std::string & s) {
    return s;
}

void Test::session() {

    typedef uint ID;

    SessionSpy<ID> spy;
    Session<ID> session;

    // Add connection which is monitored
    ID c0 = 0;
    ConnectionSpy<ID> connectionSpy(c0);
    session.addConnection(c0, connectionSpy.sendMessageOnConnectionCallbackSlot.hook());

    // Start as observer
    session.toObserveMode();
    session.start();

    // Verify that mode message sent to peer
    QVERIFY(connectionSpy.sendMessageOnConnectionCallbackSlot.called);
    connectionSpy.reset();

    // but nothing has otherwised happned in the session
    QVERIFY(spy.blank());
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
