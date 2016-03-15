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
    // Reset regtest blockchain
    bitcoind_reset_data();

    // Prepare local temporary directory for bitcoind
    QVERIFY(make_temp_directory() == 0);

    // start bitcoin server
    QVERIFY(bitcoind_start() == 0);

    // Wait for it to start
    QVERIFY(bitcoind_wait_for_ready() == 0);

    // Generate enough blocks to make the first coinbase tx spendable
    QVERIFY(bitcoin_rpc("generate 101") == 0);
}

void Test::initTestCase() {
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

    delete _wallet;
}

void Test::cleanupTestCase() {
    // Stop bitcoind
    bitcoind_stop();
}

void Test::walletCreation() {
    // Can call private methods on wallet
    _wallet->test_method();

    // Should create a fresh new wallet
    try{
        _wallet->Create(WALLET_SEED); // Return Metadata from store ?
        QVERIFY(true);
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH));

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

}

void Test::networkMismatchOnOpeningWallet() {
    joystream::bitcoin::Store s;

    // assuming a mainnet wallet already exists
    s.create(TEST_WALLET_PATH, Coin::Network::mainnet);

    // Wallet configured for regtest should fail
    QVERIFY_EXCEPTION_THROWN(_wallet->Open(), std::exception);
}

void Test::SynchingHeaders() {

    QSignalSpy spy_blocktree_error(_wallet, SIGNAL(BlockTreeError()));
    QSignalSpy spy_headers_synched(_wallet, SIGNAL(HeadersSynched()));

    _wallet->Create(WALLET_SEED);

    _wallet->LoadBlockTree();

    // Headers should have been loaded successfully
    QVERIFY(_wallet->blockTreeLoaded());

    // And without errors
    QCOMPARE(spy_blocktree_error.count(), 0);

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_headers_synched.count() > 0, 5000);

    int32_t startingHeight = _wallet->bestHeight();

    QSignalSpy spy_tree_changed(_wallet, SIGNAL(BlockTreeChanged()));

    int32_t lastCount = spy_tree_changed.count();

    // Generate one more block
    bitcoin_rpc("generate 1");

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_tree_changed.count() > lastCount, 5000);

    // One block was mined height should increase by one
    QCOMPARE(_wallet->bestHeight(), startingHeight + 1);
}

void Test::BasicBalanceCheck() {

    QSignalSpy spy_balance_changed(_wallet, SIGNAL(BalanceChanged(uint64_t, uint64_t)));

    _wallet->LoadBlockTree();

    _wallet->Create(WALLET_SEED);

    Coin::P2PKHAddress addr = _wallet->GetReceiveAddress();

    // Send 0.005BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.005");

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    // Wait for balance to change
    QVERIFY(spy_balance_changed.wait());

    QCOMPARE(_wallet->Balance(), uint64_t(0));
    QCOMPARE(_wallet->UnconfirmedBalance(), uint64_t(500000));

    //bitcoin_rpc("generate 1");
    //QTRY_VERIFY_WITH_TIMEOUT(spy_txconfirmed.count() == 1, 5000);
    //QCOMPARE(_wallet->Balance(), uint64_t(500000));
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
