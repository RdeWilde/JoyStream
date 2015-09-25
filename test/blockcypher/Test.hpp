/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>
#include <QNetworkAccessManager>

namespace Coin {
    class Transaction;
}

namespace BlockCypher {
    class Client;
}

//#define WALLET_SEED Coin::Seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931")
#define TEST_BITCOIN_NETWORK Coin::Network::testnet3
#define TEST_BLOCKCYPHER_TOKEN "e2b5a8d634aac699b3828af27cb301a3" // Token used for unit testing

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

    void deleteWallet();

    void getWallet();

    void addAddresses();

    void addressEndpoint();

    void pushRawTransaction();

};

#endif // TEST_HPP
