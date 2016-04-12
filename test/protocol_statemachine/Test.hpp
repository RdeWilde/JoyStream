/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
    class BuyerTerms;
}
namespace protocol_statemachine {
    class CBStateMachine;
}
}

using namespace joystream;

class Test : public QObject {

    Q_OBJECT

private slots:

    // Flow through states
    void observing();
    void selling();
    void buying();

public:

    static void peerToSellMode(protocol_statemachine::CBStateMachine *, const protocol_wire::SellerTerms &, uint32_t);
    static void peerToBuyMode(protocol_statemachine::CBStateMachine *, const protocol_wire::BuyerTerms &);
    static void peerToObserveMode(protocol_statemachine::CBStateMachine *);
};



#endif // TEST_HPP
