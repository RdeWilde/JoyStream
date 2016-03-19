/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject {

    Q_OBJECT

private slots:

    //// Statemachine tests

    // Client mode change operations
    void clientToSellMode();
    void clientToBuyMode();
    void clientToObserveMode();

    // Peer announces mode change
    void peerToSellMode();
    void peerToBuyMode();
    void peerToObserveMode();

    // Flow through selling states
    void selling();

public:

private:

};

#endif // TEST_HPP
