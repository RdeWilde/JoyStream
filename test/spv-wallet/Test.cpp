/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>

#include <bitcoin/SPVWallet.hpp>

#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <boost/filesystem.hpp>

/*
 *  path to bitcoind and bitcoin-cli need to be
 *  added to the Qt project 'Build Environment' PATH variable
 */

// Helper Functions for starting stopping and issuing commands to bitcoind
int make_temp_directory() {
    return system("mkdir -p bitcoind-test/");
}

int bitcoind_start() {
    return system("bitcoind -regtest -daemon -datadir=bitcoind-test/ -rpcuser=test -rpcpassword=test");
}

int bitcoin_rpc(std::string command) {
    std::string sys_string = "bitcoin-cli -regtest -rpcuser=test -rpcpassword=test ";
    std::string full_commandline = sys_string + command;

    return system(full_commandline.c_str());
}

int bitcoind_wait_for_ready() {
    return bitcoin_rpc("-rpcwait getinfo");
}

int bitcoind_stop() {
    return bitcoin_rpc("stop");
}

int bitcoind_reset_data() {
    return system("rm -fr bitcoind-test/");
}

void Test::init_bitcoind() {
    // Leaving state of blockchain at discretion of tester..
    // Unit tests should work irrespective of the state of the chain.
    // But will fail if there are no UTXO in the bitcoind wallet to spend (make sure to generate
    // at least the first 101 blocks to make first coinbase tx spendable)
    // Reset regtest blockchain
    // bitcoind_reset_data();

    // Prepare local temporary directory for bitcoind
    QVERIFY(make_temp_directory() == 0);

    // start bitcoin server
    QVERIFY(bitcoind_start() == 0);

    // Wait for it to start
    QVERIFY(bitcoind_wait_for_ready() == 0);

    // Generate a block start unit tests with an empty mempool
    QVERIFY(bitcoin_rpc("generate 1") == 0);
}

void Test::initTestCase() {
    bitcoind_stop();
    init_bitcoind();
}

void Test::init() {
    // delete existing wallet file
    boost::filesystem::remove(TEST_WALLET_PATH);

    // delete existing blocktree file
    boost::filesystem::remove(TEST_BLOCKTREE_PATH);

    // create new testnet wallet
    _wallet = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH, TEST_BLOCKTREE_PATH, Coin::Network::regtest);
}

void Test::cleanup() {
    _wallet->StopSync();
    delete _wallet;
}

void Test::cleanupTestCase() {
    // Stop bitcoind
    bitcoind_stop();
}

void Test::walletCreation() {
    // Should create a fresh new wallet
    try{
        _wallet->Create(WALLET_SEED); // Return Metadata from store ?
        QVERIFY(true);
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH));

    // After creating the wallet, we cannot call Create() again
    QVERIFY_EXCEPTION_THROWN(_wallet->Create(), std::exception);

    delete _wallet;
    _wallet = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH, TEST_BLOCKTREE_PATH, Coin::Network::regtest);

    // Should throw exception if we try to create wallet over existing file
    QVERIFY_EXCEPTION_THROWN(_wallet->Create(), std::exception);

    // Should open existing wallet
    try {
        _wallet->Open();
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Check correct metadata values of created wallet
    QCOMPARE(_wallet->network(), Coin::Network::regtest);

    // After opening a wallet, we cannot call Open() again
    QVERIFY_EXCEPTION_THROWN(_wallet->Open(), std::exception);

}

void Test::networkMismatchOnOpeningWallet() {
    joystream::bitcoin::Store s;

    // assuming a mainnet wallet already exists
    s.create(TEST_WALLET_PATH, Coin::Network::mainnet);

    // Wallet configured for regtest should fail
    QVERIFY_EXCEPTION_THROWN(_wallet->Open(), std::exception);
}

void Test::SynchingHeaders() {

    QSignalSpy spy_headers_synched(_wallet, SIGNAL(HeadersSynched()));
    QSignalSpy spy_tree_changed(_wallet, SIGNAL(BlockTreeChanged()));

    _wallet->Create(WALLET_SEED);

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_headers_synched.count() > 0, 10000);

    int32_t startingHeight = _wallet->bestHeight();

    int32_t lastCount = spy_tree_changed.count();

    // Generate one more block
    bitcoin_rpc("generate 1");

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_tree_changed.count() > lastCount, 10000);

    // One block was mined height should increase by one
    QCOMPARE(_wallet->bestHeight(), startingHeight + 1);

    lastCount = spy_tree_changed.count();

    // Generate one more block
    bitcoin_rpc("generate 1");

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_tree_changed.count() > lastCount, 10000);

    // One block was mined height should increase by one
    QCOMPARE(_wallet->bestHeight(), startingHeight + 2);
}

void Test::BalanceCheck() {

    QSignalSpy spy_balance_changed(_wallet, SIGNAL(BalanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_blocks_synched(_wallet, SIGNAL(BlocksSynched()));

    _wallet->Create(WALLET_SEED);

    Coin::P2PKHAddress addr = _wallet->GetReceiveAddress();

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, 10000);

    uint64_t startingConfirmedBalance = _wallet->Balance();
    uint64_t startingUnconfirmedBalance = _wallet->UnconfirmedBalance();

    int lastBalanceChangeCount = spy_balance_changed.count();

    // Send 0.005BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.005");

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, 15000);

    QCOMPARE(_wallet->Balance(), startingConfirmedBalance);
    QCOMPARE(_wallet->UnconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(500000)));

    lastBalanceChangeCount = spy_balance_changed.count();

    // Generate a block to confirm the last transaction
    bitcoin_rpc("generate 1");

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, 15000);

    lastBalanceChangeCount = spy_balance_changed.count();

    // Send another 0.005BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.005");

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, 15000);

    QCOMPARE(_wallet->Balance(), uint64_t(startingConfirmedBalance + uint64_t(500000)) );
    QCOMPARE(_wallet->UnconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(1000000)));

    // Simulate effect of a reorg

    int startAtHeight = _wallet->bestHeight() - 1;
    // Send another 0.003BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.003");
    bitcoin_rpc("generate 3");

    _wallet->test_syncBlocksStaringAtHeight(startAtHeight);
    QTest::qWait(15000);

    QCOMPARE(_wallet->Balance(), uint64_t(startingConfirmedBalance + uint64_t(1300000)) );

    _wallet->StopSync();
}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
