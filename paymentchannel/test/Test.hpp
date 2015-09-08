/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>

namespace Coin {
    class Transaction;
}

#define WALLET_FILE_NAME "test-paymentchannel"
#define WALLET_SEED Coin::Seed::testSeeds[0]
#define NETWORK_TYPE Coin::Network::testnet3


namespace Wallet {
    class Manager;
}

class Test : public QObject
{
    Q_OBJECT

    //Wallet::Manager * _manager;

private slots:

    // called before *each* non-built in test case
    void init();

    // called after *each* non-built in test case
    void cleanup();

    /**
     * Test routines
     */

};

#endif // TESTWALLET_HPP
