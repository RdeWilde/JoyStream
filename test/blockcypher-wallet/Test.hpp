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
    class WebSocketClient;
}

namespace joystream {
namespace bitcoin {
    class BlockCypherWallet;
}
}

#define WALLET_SEED Coin::Seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931")
#define TEST_BITCOIN_NETWORK Coin::Network::testnet3
#define TEST_BLOCKCYPHER_TOKEN "e2b5a8d634aac699b3828af27cb301a3" // Token used for unit testing
#define TEST_WALLET_PATH "test-wallet.db"

class Test : public QObject
{
    Q_OBJECT

    QNetworkAccessManager _manager;

    BlockCypher::Client * _client;

    BlockCypher::WebSocketClient *_wsClient;

    joystream::bitcoin::BlockCypherWallet *_wallet;

private slots:

    // Will be called before each testfunction is executed
    void init();

    // Will be called after every testfunction.
    void cleanup();

    /**
     * Real test cases
     */

    void walletCreation();
    void fundWallet();
    void getUtxo();

    void networkMismatchOnCreatingWallet();
    void networkMismatchOnOpeningWallet();
};

#endif // TEST_HPP
