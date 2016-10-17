/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <Test.hpp>

#include "bitcoin_regtest_framework.hpp"

#include <bitcoin/SPVWallet.hpp>
#include <bitcoin/Common.hpp>

#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/UnspentOutput.hpp>
#include <common/UnspentOutputSet.hpp>
#include <common/Utilities.hpp>
#include <common/Entropy.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <boost/filesystem.hpp>
#include <boost/asio/impl/src.hpp>

using namespace joystream::test::bitcoin;

#define DEFAULT_SIGNAL_TIMEOUT 20 * 1000

void Test::initTestCase() {

    QVERIFY(regtest::init() == 0);

    // SPVWallet - CoinQ netsync log
    if(getenv("NETSYNC_LOGGER") != NULL) {
        INIT_LOGGER("sync.log");
    }
}

void Test::init() {
    // delete existing wallet files
    boost::filesystem::remove(TEST_WALLET_PATH_A);
    boost::filesystem::remove(TEST_WALLET_PATH_B);

    // delete existing blocktree file
    boost::filesystem::remove(TEST_BLOCKTREE_PATH_A);
    boost::filesystem::remove(TEST_BLOCKTREE_PATH_B);

    // create new testnet wallet
    _walletA = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    _walletB = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH_B, TEST_BLOCKTREE_PATH_B, Coin::Network::regtest);
}

void Test::cleanup() {
    _walletA->stopSync();
    delete _walletA;
    delete _walletB;
}

void Test::cleanupTestCase() {
    // Stop bitcoind
    regtest::shutdown();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
