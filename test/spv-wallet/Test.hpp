/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>
namespace joystream {
namespace bitcoin {
    class SPVWallet;
}
}

#define WALLET_SEED Coin::Seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931")
#define TEST_BITCOIN_NETWORK Coin::Network::regtest
#define TEST_WALLET_PATH "test-wallet.db"
#define TEST_BLOCKTREE_PATH "test-blocktree.dat"

class Test : public QObject
{
    Q_OBJECT

    joystream::bitcoin::SPVWallet *_wallet;

    void init_bitcoind();
    void cleanup_bitcoind();

    bool _bitcoind_started;

private slots:

    // Will be called before each testfunction is executed
    void init();

    // Will be called after every testfunction.
    void cleanup();

    // Test cases
    void walletCreation();
    void networkMismatchOnOpeningWallet();

    void SynchingHeaders();
};

#endif // TEST_HPP
