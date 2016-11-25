/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
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
}

void Test::cleanup() {

}

void Test::cleanupTestCase() {
    // Stop bitcoind
    regtest::shutdown();
}

void Test::walletCreation() {

    // no wallet file exists
    QVERIFY(!boost::filesystem::exists(TEST_WALLET_PATH_A));

    // Should create a fresh new wallet
    try{
        joystream::bitcoin::SPVWallet w(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::mainnet);
    } catch(...) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH_A));

    // Re-open existing wallet
    try {
        joystream::bitcoin::SPVWallet w(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::mainnet);
        // Check correct metadata values of created wallet
        QCOMPARE(w.network(), Coin::Network::mainnet);
    } catch(...) {
        QVERIFY(false);
    }

}

void Test::walletNetworkMismatch() {

    // no wallet file exists
    QVERIFY(!boost::filesystem::exists(TEST_WALLET_PATH_A));

    // Should create a fresh new wallet
    try{
        joystream::bitcoin::SPVWallet w(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    } catch(...) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH_A));

    // Re-open existing wallet with wrong network parameter should throw
    try {
        joystream::bitcoin::SPVWallet w(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::mainnet);
        QVERIFY(false);
    } catch(...) {

    }
}

void Test::extendedPublicKey() {

    joystream::bitcoin::Store store1(TEST_WALLET_PATH_A, Coin::Network::regtest);

    auto privKey1 = store1.derivePrivateKey(joystream::bitcoin::Store::KeychainType::External, 5);
    auto pubKey1 = store1.derivePublicKey(joystream::bitcoin::Store::KeychainType::External, 5);

    QVERIFY(privKey1.toPublicKey() == pubKey1);

    joystream::bitcoin::Store store2(TEST_WALLET_PATH_A, Coin::Network::regtest);
    auto privKey2 = store2.derivePrivateKey(joystream::bitcoin::Store::KeychainType::External, 5);
    auto pubKey2 = store2.derivePublicKey(joystream::bitcoin::Store::KeychainType::External, 5);

    QVERIFY(privKey2.toPublicKey() == pubKey2);

    QVERIFY(privKey1 == privKey2);
    QVERIFY(pubKey1 == pubKey2);
}

void Test::walletEncryption() {

    std::string seed;

    {
        joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
        seed = walletA.getSeedWords();
        walletA.encrypt("password");
    }

    // Opening an encrypted wallet without a passphrase it will be in locked state
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    QVERIFY(walletA.locked());
    QVERIFY_EXCEPTION_THROWN(walletA.getSeedWords(), std::exception);

    QVERIFY_EXCEPTION_THROWN(walletA.decrypt("wrongPassword"), std::exception);
    QVERIFY(walletA.locked());

    walletA.decrypt("password");
    QVERIFY(seed == walletA.getSeedWords());
}

void Test::walletLocking() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    QVERIFY(!walletA.locked());
    QVERIFY(!walletA.encrypted());

    walletA.encrypt("password");
    QVERIFY(walletA.encrypted());
    QVERIFY(!walletA.locked());

    walletA.lock();
    QVERIFY(walletA.locked());

    walletA.decrypt("password");
    QVERIFY(!walletA.encrypted());
    QVERIFY(!walletA.locked());
}

void Test::Synching() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    QSignalSpy spy_blocks_synched(&walletA, SIGNAL(synched()));
    QSignalSpy spy_store_error(&walletA, SIGNAL(storeUpdateFailed(std::string)));

    // Should connect and synch headers
    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    int32_t startingHeight = walletA.bestHeight();

    int32_t lastCount = spy_blocks_synched.count();

    // Generate one more block
    QVERIFY(regtest::generate_blocks(1) == 0);

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, DEFAULT_SIGNAL_TIMEOUT);

    // One block was mined height should increase by one
    QCOMPARE(walletA.bestHeight(), startingHeight + 1);

    lastCount = spy_blocks_synched.count();

    // Generate one more block
    QVERIFY(regtest::generate_blocks(1) == 0);

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, DEFAULT_SIGNAL_TIMEOUT);

    // One block was mined height should increase by one
    QCOMPARE(walletA.bestHeight(), startingHeight + 2);

    QCOMPARE(walletA.bestHeight(), walletA.test_netsyncBestHeight());

    // Make sure there were no store errors
    QCOMPARE(spy_store_error.count(), 0);
}

void Test::BalanceCheck() {

    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    QSignalSpy spy_balance_changed(&walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_blocks_synched(&walletA, SIGNAL(synched()));
    QSignalSpy spy_store_error(&walletA, SIGNAL(storeUpdateFailed(std::string)));

    auto addr = walletA.generateReceiveAddress();

    // Should connect and synch headers
    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    uint64_t startingConfirmedBalance = walletA.balance();
    uint64_t startingUnconfirmedBalance = walletA.unconfirmedBalance();

    int lastBalanceChangeCount = spy_balance_changed.count();

    // Send 0.005BTC to our wallet
    QVERIFY(regtest::send_to_address(addr.toBase58CheckEncoding().toStdString(), "0.005") == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(walletA.balance(), startingConfirmedBalance);
    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(500000)));

    lastBalanceChangeCount = spy_balance_changed.count();

    // Generate a block to confirm the last transaction
    QVERIFY(regtest::generate_blocks(1) == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, DEFAULT_SIGNAL_TIMEOUT);

    lastBalanceChangeCount = spy_balance_changed.count();

    // Send another 0.005BTC to our wallet
    QVERIFY(regtest::send_to_address(addr.toBase58CheckEncoding().toStdString(), "0.005") == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(walletA.balance(), uint64_t(startingConfirmedBalance + uint64_t(500000)) );
    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(1000000)));

    // Simulate effect of a reorg

    int startAtHeight = walletA.bestHeight() - 1;
    // Send another 0.003BTC to our wallet
    QVERIFY(regtest::send_to_address(addr.toBase58CheckEncoding().toStdString(), "0.003") == 0);
    QVERIFY(regtest::generate_blocks(3) == 0);

    walletA.test_syncBlocksStaringAtHeight(startAtHeight);
    QTest::qWait(15000);

    QCOMPARE(walletA.balance(), uint64_t(startingConfirmedBalance + uint64_t(1300000)) );

    walletA.stopSync();

    // Make sure there were no store errors
    QCOMPARE(spy_store_error.count(), 0);
}

void Test::Utxo() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    auto addr1 = walletA.generateReceiveAddress();
    auto addr2 = walletA.generateReceiveAddress();
    auto addr3 = walletA.generateReceiveAddress();

    // 100,000 satoshi (2 conf)
    QVERIFY(regtest::send_to_address(addr1.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    //  50,000 satoshi (1 conf)
    QVERIFY(regtest::send_to_address(addr2.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);


    QSignalSpy spy_blocks_synched(&walletA, SIGNAL(synched()));

    // Should connect and synch headers
    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changed(&walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    //  25,000 satoshi (0 conf)
    QVERIFY(regtest::send_to_address(addr3.toBase58CheckEncoding().toStdString(), "0.00025") == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(175000));

    Coin::UnspentOutputSet lockedOutputs;

    // Check existence of all UTXOs
    {
        auto utxos(walletA.lockOutputs(100000, 2));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(100000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr1.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    {
        auto utxos(walletA.lockOutputs(50000, 1));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(50000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr2.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    // An empty utxo list should be returned when not enough funds available
    {
        auto utxos(walletA.lockOutputs(100000, 0));
        QCOMPARE(int(utxos.size()), 0);
    }

    {
        auto utxos(walletA.lockOutputs(25000, 0));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(25000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr3.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    // Outputs can be unlocked
    QCOMPARE(walletA.unlockOutputs(lockedOutputs), uint(3));


    /* Largest Utxos are returned first
    {
        auto utxos(walletA.lockOutputs(175000, 0));
        QCOMPARE(int(utxos.size()), 3);
        auto i = utxos.begin();
        QCOMPARE(uint64_t((*i)->value()), uint64_t(100000));
        i++;
        QCOMPARE(uint64_t((*i)->value()), uint64_t(50000));
        i++;
        QCOMPARE(uint64_t((*i)->value()), uint64_t(25000));
    }
    */
}

void Test::BroadcastingTx() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    joystream::bitcoin::SPVWallet walletB(TEST_WALLET_PATH_B, TEST_BLOCKTREE_PATH_B, Coin::Network::regtest);

    auto addrA = walletA.generateReceiveAddress();
    auto addrB = walletB.generateReceiveAddress();

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(&walletA, SIGNAL(synched()));

    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy synchedB(&walletB, SIGNAL(synched()));

    walletB.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(&walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_balance_changedB(&walletB, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    walletA.test_sendToAddress(70000, addrB, 5000);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(walletB.unconfirmedBalance(), uint64_t(70000));

    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(25000));

}

void Test::UsingOptionalDataInP2SHSpend() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    // script = OP_ADD 2 OP_EQUALVERIFY
    uchar_vector script;
    script.push_back(0x93); script.push_back(0x52); script.push_back(0x88);

    // data:  OP_1 OP_1
    uchar_vector data;
    data.push_back(0x51); data.push_back(0x51);

    Coin::PrivateKey key(walletA.generateKey([&script, &data](const Coin::PublicKey &pubkey){
        // Push Public Key to script
        script += Coin::opPushData(0x21);
        script += pubkey.toUCharVector();

        script.push_back(0xac); // OP_CHECKSIG

        return joystream::bitcoin::RedeemScriptInfo(script, data);
    }));

    Coin::P2SHAddress addrA = Coin::P2SHAddress(Coin::Network::regtest, Coin::RedeemScriptHash::fromRawScript(script));

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(&walletA, SIGNAL(synched()));

    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(&walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(100000));

    {
        auto utxos = walletA.lockOutputs(0, 0);
        // Our custom p2sh output should not be recognised as a standard output
        QCOMPARE(uint(utxos.size()), uint(0));
        walletA.unlockOutputs(utxos);
    }

    // Selector for our custom p2sh output
    auto selector = [&script, &data](const joystream::bitcoin::Store::StoreControlledOutput &output) -> Coin::UnspentOutput* {
            if(output.redeemScript == script && output.optionalData == data) {
                return new Coin::UnspentP2SHOutput(output.keyPair,
                                                   output.redeemScript,
                                                   output.optionalData,
                                                   output.outPoint,
                                                   output.value);
            }

            return nullptr;
    };

    {
        auto utxos = walletA.lockOutputs(0, 0, selector);
        QCOMPARE(uint(utxos.size()), uint(1));
        walletA.unlockOutputs(utxos);
    }

    walletA.test_sendToAddress(10000, addrA, 5000, selector);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(95000));
}

void Test::FinanceTxFromMultipleSets() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    joystream::bitcoin::SPVWallet walletB(TEST_WALLET_PATH_B, TEST_BLOCKTREE_PATH_B, Coin::Network::regtest);

    auto addrA = walletA.generateReceiveAddress();
    auto addrB = walletB.generateReceiveAddress();

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(&walletA, SIGNAL(synched()));

    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy synchedB(&walletB, SIGNAL(synched()));

    walletB.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(&walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_balance_changedB(&walletB, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    auto utxoSet1(walletA.lockOutputs(50000));
    auto utxoSet2(walletA.lockOutputs(50000));

    Coin::Transaction tx;

    tx.addOutput(Coin::TxOut(70000, Coin::P2PKHScriptPubKey(addrB.pubKeyHash()).serialize())); // to walletB
    tx.addOutput(Coin::TxOut(25000, Coin::P2PKHScriptPubKey(addrA.pubKeyHash()).serialize())); // change

    utxoSet1.finance(tx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, true));
    utxoSet2.finance(tx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, true));

    walletA.broadcastTx(tx);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(walletB.unconfirmedBalance(), uint64_t(70000));

    QCOMPARE(walletA.unconfirmedBalance(), uint64_t(25000));
}

void Test::RedeemScriptFiltering() {
    joystream::bitcoin::SPVWallet walletA(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);

    // script = OP_ADD 2 OP_EQUALVERIFY
    uchar_vector script;
    script.push_back(0x93); script.push_back(0x52); script.push_back(0x88);

    Coin::PrivateKey key(walletA.generateKey([&script](const Coin::PublicKey &pubkey){
        return script;
    }));

    Coin::P2SHAddress addrA = Coin::P2SHAddress(Coin::Network::regtest, Coin::RedeemScriptHash::fromRawScript(script));

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(&walletA, SIGNAL(synched()));

    walletA.sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    auto selector1 = [&script](const joystream::bitcoin::Store::StoreControlledOutput &output) -> Coin::UnspentOutput* {
        return nullptr;
    };

    auto outputs1 = walletA.lockOutputs(10000,0, selector1);

    QCOMPARE(uint(outputs1.size()), uint(0));


    auto selector2 = [&script](const joystream::bitcoin::Store::StoreControlledOutput &output) -> Coin::UnspentOutput* {

        if(script == output.redeemScript) {
            return new Coin::UnspentP2SHOutput(output.keyPair, output.redeemScript, output.optionalData, output.outPoint, output.value);
        }

        return nullptr;
    };

    auto outputs2 = walletA.lockOutputs(10000,0, selector2);

    QCOMPARE(uint(outputs2.size()), uint(1));

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
