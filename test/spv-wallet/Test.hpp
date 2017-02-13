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

#define TEST_BITCOIN_NETWORK Coin::Network::regtest
#define TEST_WALLET_PATH_A "test-wallet_1.db"
#define TEST_BLOCKTREE_PATH_A "test-blocktree_1.dat"
#define TEST_WALLET_PATH_B "test-wallet_2.db"
#define TEST_BLOCKTREE_PATH_B "test-blocktree_2.dat"

class Test : public QObject
{
    Q_OBJECT

private:
    joystream::bitcoin::SPVWallet *_walletA;
    joystream::bitcoin::SPVWallet *_walletB;

private slots:

    // Will be called once at the start
    void initTestCase();

    // Will be called before each unit test is executed
    void init();

    // Will be called after every unit test
    void cleanup();

    // Unit Tests
    void walletCreation();
    void walletNetworkMismatch();
    void extendedPublicKey();
    void walletEncryption();
    void walletLocking();
    void Synching();
    void BalanceCheck();
    void Utxo();
    void BroadcastingTx();
    void UsingOptionalDataInP2SHSpend();
    void FinanceTxFromMultipleSets();
    void RedeemScriptFiltering();

    // Will be called at the end of all tests
    void cleanupTestCase();
};

#endif // TEST_HPP
