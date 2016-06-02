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
#include <common/UnspentOutput.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <boost/filesystem.hpp>
#include <boost/asio/impl/src.hpp>

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
    bitcoind_stop();
}

void Test::walletCreation() {
    // Should create a fresh new wallet
    try{
        _walletA->create(WALLET_SEED); // Return Metadata from store ?
        QVERIFY(true);
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH_A));

    // After creating the wallet, we cannot call Create() again
    QVERIFY_EXCEPTION_THROWN(_walletA->create(), std::exception);

    delete _walletA;
    _walletA = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    // Should throw exception if we try to create wallet over existing file
    QVERIFY_EXCEPTION_THROWN(_walletA->create(), std::exception);

    // Should open existing wallet
    try {
        _walletA->open();
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Check correct metadata values of created wallet
    QCOMPARE(_walletA->network(), Coin::Network::regtest);

    // After opening a wallet, we cannot call Open() again
    QVERIFY_EXCEPTION_THROWN(_walletA->open(), std::exception);

}

void Test::networkMismatchOnOpeningWallet() {
    joystream::bitcoin::Store s;

    // assuming a mainnet wallet already exists
    s.create(TEST_WALLET_PATH_A, Coin::Network::mainnet);

    // Wallet configured for regtest should fail
    QVERIFY_EXCEPTION_THROWN(_walletA->open(), std::exception);
}

void Test::Synching() {

    QSignalSpy spy_blocks_synched(_walletA, SIGNAL(synched()));
    QSignalSpy spy_store_error(_walletA, SIGNAL(storeUpdateFailed(std::string)));

    _walletA->create(WALLET_SEED);

    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, 10000);

    int32_t startingHeight = _walletA->bestHeight();

    int32_t lastCount = spy_blocks_synched.count();

    // Generate one more block
    bitcoin_rpc("generate 1");

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, 10000);

    // One block was mined height should increase by one
    QCOMPARE(_walletA->bestHeight(), startingHeight + 1);

    lastCount = spy_blocks_synched.count();

    // Generate one more block
    bitcoin_rpc("generate 1");

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, 10000);

    // One block was mined height should increase by one
    QCOMPARE(_walletA->bestHeight(), startingHeight + 2);

    QCOMPARE(_walletA->bestHeight(), _walletA->test_netsyncBestHeight());

    // Make sure there were no store errors
    QCOMPARE(spy_store_error.count(), 0);
}

void Test::BalanceCheck() {

    QSignalSpy spy_balance_changed(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_blocks_synched(_walletA, SIGNAL(synched()));
    QSignalSpy spy_store_error(_walletA, SIGNAL(storeUpdateFailed(std::string)));

    _walletA->create(WALLET_SEED);

    Coin::P2SHAddress addr = _walletA->generateReceiveAddress();

    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, 10000);

    uint64_t startingConfirmedBalance = _walletA->balance();
    uint64_t startingUnconfirmedBalance = _walletA->unconfirmedBalance();

    int lastBalanceChangeCount = spy_balance_changed.count();

    // Send 0.005BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.005");

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, 15000);

    QCOMPARE(_walletA->balance(), startingConfirmedBalance);
    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(500000)));

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

    QCOMPARE(_walletA->balance(), uint64_t(startingConfirmedBalance + uint64_t(500000)) );
    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(1000000)));

    // Simulate effect of a reorg

    int startAtHeight = _walletA->bestHeight() - 1;
    // Send another 0.003BTC to our wallet
    bitcoin_rpc("sendtoaddress " + addr.toBase58CheckEncoding().toStdString() + " 0.003");
    bitcoin_rpc("generate 3");

    _walletA->test_syncBlocksStaringAtHeight(startAtHeight);
    QTest::qWait(15000);

    QCOMPARE(_walletA->balance(), uint64_t(startingConfirmedBalance + uint64_t(1300000)) );

    _walletA->stopSync();

    // Make sure there were no store errors
    QCOMPARE(spy_store_error.count(), 0);
}

void Test::Utxo() {
    _walletA->create();

    Coin::P2SHAddress addr1 = _walletA->generateReceiveAddress();
    Coin::P2SHAddress addr2 = _walletA->generateReceiveAddress();
    Coin::P2SHAddress addr3 = _walletA->generateReceiveAddress();

    bitcoin_rpc("sendtoaddress " + addr1.toBase58CheckEncoding().toStdString() + " 0.00100"); // 100,000 satoshi (2 conf)
    bitcoin_rpc("generate 1");
    bitcoin_rpc("sendtoaddress " + addr2.toBase58CheckEncoding().toStdString() + " 0.00050"); //  50,000 satoshi (1 conf)
    bitcoin_rpc("generate 1");


    QSignalSpy spy_blocks_synched(_walletA, SIGNAL(synched()));

    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, 10000);

    QSignalSpy spy_balance_changed(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    bitcoin_rpc("sendtoaddress " + addr3.toBase58CheckEncoding().toStdString() + " 0.00025"); //  25,000 satoshi (0 conf)

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > 0, 10000);

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(175000));

    std::list<std::shared_ptr<Coin::UnspentOutput>> lockedOutputs;

    // Check existence of all UTXOs
    {
        auto utxos(_walletA->lockOutputs(100000, 2));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.front()->value()), uint64_t(100000));
        QCOMPARE(utxos.front()->scriptPubKey().getHex(), addr1.toP2SHScriptPubKey().serialize().getHex());
        lockedOutputs.push_back(utxos.front());
    }

    {
        auto utxos(_walletA->lockOutputs(50000, 1));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.front()->value()), uint64_t(50000));
        QCOMPARE(utxos.front()->scriptPubKey().getHex(), addr2.toP2SHScriptPubKey().serialize().getHex());
        lockedOutputs.push_back(utxos.front());
    }

    // An empty utxo list should be returned when not enough funds available
    {
        auto utxos(_walletA->lockOutputs(100000, 0));
        QCOMPARE(int(utxos.size()), 0);
    }

    {
        auto utxos(_walletA->lockOutputs(25000, 0));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.front()->value()), uint64_t(25000));
        QCOMPARE(utxos.front()->scriptPubKey().getHex(), addr3.toP2SHScriptPubKey().serialize().getHex());
        lockedOutputs.push_back(utxos.front());
    }

    // Outputs can be unlocked
    _walletA->unlockOutputs(lockedOutputs);

    // Largest Utxos are returned first
    {
        auto utxos(_walletA->lockOutputs(175000, 0));
        QCOMPARE(int(utxos.size()), 3);
        auto i = utxos.begin();
        QCOMPARE(uint64_t((*i)->value()), uint64_t(100000));
        i++;
        QCOMPARE(uint64_t((*i)->value()), uint64_t(50000));
        i++;
        QCOMPARE(uint64_t((*i)->value()), uint64_t(25000));
    }

}

void Test::BroadcastingTx() {
    _walletA->create();
    _walletB->create();

    Coin::P2SHAddress addrA = _walletA->generateReceiveAddress();
    Coin::P2SHAddress addrB = _walletB->generateReceiveAddress();

    bitcoin_rpc("sendtoaddress " + addrA.toBase58CheckEncoding().toStdString() + " 0.00100");
    bitcoin_rpc("generate 1");

    QSignalSpy synchedA(_walletA, SIGNAL(synched()));

    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, 10000);

    QSignalSpy synchedB(_walletB, SIGNAL(synched()));

    _walletB->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedB.count() > 0, 10000);

    QSignalSpy spy_balance_changedA(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_balance_changedB(_walletB, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    _walletA->test_sendToAddress(50000, addrB, 1000);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, 5000);
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedB.count() > 0, 5000);

    QCOMPARE(_walletB->unconfirmedBalance(), uint64_t(50000));

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(49000));

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
