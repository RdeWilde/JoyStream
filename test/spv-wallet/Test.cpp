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

    // Prepare local temporary directory for bitcoind
    QVERIFY(make_temp_directory() == 0);

    // start bitcoin server
    QVERIFY(bitcoind_start() == 0);

    _bitcoind_started = true;

    // Wait for it to start
    QVERIFY(bitcoind_wait_for_ready() == 0);
}

void Test::cleanup_bitcoind() {
    _bitcoind_started = false;

    // Stop bitcoind
    QVERIFY(bitcoind_stop() == 0);

    // Wait for it to shutdown... (when it stops responding to rpc requests)
    const int timeout = 500;
    QTRY_VERIFY_WITH_TIMEOUT(bitcoin_rpc("getinfo") != 0, timeout);

    // Reset regtest blockchain
    bitcoind_reset_data();
}

void Test::init() {
    // delete existing wallet file
    boost::filesystem::remove(TEST_WALLET_PATH);

    // create new testnet wallet
    _wallet = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH, Coin::Network::regtest);
}

void Test::cleanup() {

    delete _wallet;

    if(_bitcoind_started) cleanup_bitcoind();
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

void Test::bitcoind() {
    init_bitcoind();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
