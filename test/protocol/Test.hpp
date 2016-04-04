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
namespace protocol {

    class SellerTerms;
    class BuyerTerms;

    namespace statemachine {
        class CBStateMachine;
    }
}
}

using namespace joystream::protocol;

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

    static void peerToSellMode(statemachine::CBStateMachine *, const SellerTerms &, uint32_t);
    static void peerToBuyMode(statemachine::CBStateMachine *, const BuyerTerms &);
    static void peerToObserveMode(statemachine::CBStateMachine *);
};



#endif // TEST_HPP
