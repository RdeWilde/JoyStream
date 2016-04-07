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
namespace wire {
    class SellerTerms;
    class BuyerTerms;
}
namespace protocol {
namespace statemachine {
    class CBStateMachine;
}
}
}

using namespace joystream::protocol::statemachine;

class Test : public QObject {

    Q_OBJECT

private slots:

    // Flow through states
    void observing();
    void selling();
    void buying();

    // A buyer interacting with a set of sellers
    void buyerAndSellers();

public:

    static void peerToSellMode(CBStateMachine *, const joystream::wire::SellerTerms &, uint32_t);
    static void peerToBuyMode(CBStateMachine *, const joystream::wire::BuyerTerms &);
    static void peerToObserveMode(CBStateMachine *);
};



#endif // TEST_HPP
