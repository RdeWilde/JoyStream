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
    if(_bitcoind_started) return;

    // Prepare local temporary directory for bitcoind
    QVERIFY(make_temp_directory() == 0);

    // start bitcoin server
    QVERIFY(bitcoind_start() == 0);

    _bitcoind_started = true;

    // Wait for it to start
    QVERIFY(bitcoind_wait_for_ready() == 0);
}

void Test::cleanup_bitcoind() {
    if(!_bitcoind_started) return;
    _bitcoind_started = false;

    // Stop bitcoind
    QVERIFY(bitcoind_stop() == 0);

    // Wait for it to shutdown... (when it stops responding to rpc requests)
    const int timeout = 5000;
    QTRY_VERIFY_WITH_TIMEOUT(bitcoin_rpc("getinfo") != 0, timeout);

    // Reset regtest blockchain
    bitcoind_reset_data();
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

Test::~Test() {
    cleanup_bitcoind();
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
    init_bitcoind();

    // Generate one block
    bitcoin_rpc("generate 1");

    QSignalSpy spy_blocktree_error(_wallet, SIGNAL(BlockTreeError()));
    QSignalSpy spy_synching_headers(_wallet, SIGNAL(SynchingHeaders()));
    QSignalSpy spy_headers_synched(_wallet, SIGNAL(HeadersSynched()));

    _wallet->Create(WALLET_SEED);

    _wallet->LoadBlockTree();

    // Headers should have been loaded successfully
    QVERIFY(_wallet->blockTreeLoaded());

    // And without errors
    QCOMPARE(spy_blocktree_error.count(), 0);

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_synching_headers.count() == 1, 1000); // 1s timeout
    QTRY_VERIFY_WITH_TIMEOUT(spy_headers_synched.count() == 1, 1000); // 1s timeout

    // One block mined after the genesis block so best height should be equal to 1
    QCOMPARE(_wallet->bestHeight(), 1);
}

void Test::BasicBalanceCheck() {
    init_bitcoind();

    QSignalSpy spy_newtx(_wallet, SIGNAL(NewTx()));
    QSignalSpy spy_txconfirmed(_wallet, SIGNAL(TxConfirmed()));

    _wallet->LoadBlockTree();

    _wallet->Create(WALLET_SEED);

    Coin::P2PKHAddress addr = _wallet->GetReceiveAddress();

    // Generate enough blocks to make the first coinbase tx spendable
    bitcoin_rpc("generate 101");

    // Send 0.005BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.005");

    // Should connect and synch headers
    _wallet->Sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_newtx.count() == 1, 5000);
    QCOMPARE(_wallet->Balance(), uint64_t(0));
    QCOMPARE(_wallet->UnconfirmedBalance(), uint64_t(500000));

    //bitcoin_rpc("generate 1");
    //QTRY_VERIFY_WITH_TIMEOUT(spy_txconfirmed.count() == 1, 5000);
    //QCOMPARE(_wallet->Balance(), uint64_t(500000));
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
