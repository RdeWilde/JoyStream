/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>
#include <QNetworkAccessManager>

namespace Coin {
    class Transaction;
}

namespace BlockCypher {
    class Client;
}

#define WALLET_SEED Coin::Seed::testSeeds[0]
#define NETWORK_TYPE Coin::Network::testnet3

class Test : public QObject
{
    Q_OBJECT

    QNetworkAccessManager _manager;

    BlockCypher::Client * _client;

private slots:

    // Will be called before each testfunction is executed
    void init();

    // Will be called after every testfunction.
    void cleanup();

    /**
     * Real test cases
     */

    void createWallet();

    void address();

    void txref();

    void wallet();
};

#endif // TESTWALLET_HPP
