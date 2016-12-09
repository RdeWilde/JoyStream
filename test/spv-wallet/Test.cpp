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

void Test::walletCreation() {
    // Should create a fresh new wallet
    try{
        _walletA->create();
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

void Test::walletNetworkMismatch() {

    // create default regtest wallet
    _walletA->create();
    delete _walletA;

    // init a mainnet wallet
    _walletA = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::mainnet);

    // Attempting to open wallet with wrong network type should throw
    QVERIFY_EXCEPTION_THROWN(_walletA->open(), std::exception);
}

void Test::extendedPublicKey() {
    auto store = new joystream::bitcoin::Store();
    store->create(TEST_WALLET_PATH_A, Coin::Network::regtest);

    auto privKey1 = store->derivePrivateKey(joystream::bitcoin::Store::KeychainType::External, 5);
    auto pubKey1 = store->derivePublicKey(joystream::bitcoin::Store::KeychainType::External, 5);

    QVERIFY(privKey1.toPublicKey() == pubKey1);

    delete store;

    store = new joystream::bitcoin::Store(TEST_WALLET_PATH_A, Coin::Network::regtest);
    auto privKey2 = store->derivePrivateKey(joystream::bitcoin::Store::KeychainType::External, 5);
    auto pubKey2 = store->derivePublicKey(joystream::bitcoin::Store::KeychainType::External, 5);

    QVERIFY(privKey2.toPublicKey() == pubKey2);

    QVERIFY(privKey1 == privKey2);
    QVERIFY(pubKey1 == pubKey2);

    delete store;
}

void Test::walletEncryption() {
    _walletA->create();
    std::string seed = _walletA->getSeedWords();

    _walletA->encrypt("password");
    delete _walletA;

    // Opening an encrypted wallet without a passphrase it will be in locked state
    _walletA = new joystream::bitcoin::SPVWallet(TEST_WALLET_PATH_A, TEST_BLOCKTREE_PATH_A, Coin::Network::regtest);
    _walletA->open();

    QVERIFY(_walletA->locked());
    QVERIFY_EXCEPTION_THROWN(_walletA->getSeedWords(), std::exception);

    QVERIFY_EXCEPTION_THROWN(_walletA->decrypt("wrongPassword"), std::exception);
    QVERIFY(_walletA->locked());

    _walletA->decrypt("password");
    QVERIFY(seed == _walletA->getSeedWords());
}

void Test::walletLocking() {
    _walletA->create();
    QVERIFY(!_walletA->locked());
    QVERIFY(!_walletA->encrypted());

    _walletA->encrypt("password");
    QVERIFY(_walletA->encrypted());
    QVERIFY(!_walletA->locked());

    _walletA->lock();
    QVERIFY(_walletA->locked());

    _walletA->decrypt("password");
    QVERIFY(!_walletA->encrypted());
    QVERIFY(!_walletA->locked());
}

void Test::Synching() {

    QSignalSpy spy_blocks_synched(_walletA, SIGNAL(synched()));
    QSignalSpy spy_store_error(_walletA, SIGNAL(storeUpdateFailed(std::string)));

    //_walletA->create(WALLET_SEED);
    _walletA->create();
    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    int32_t startingHeight = _walletA->bestHeight();

    int32_t lastCount = spy_blocks_synched.count();

    // Generate one more block
    QVERIFY(regtest::generate_blocks(1) == 0);

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, DEFAULT_SIGNAL_TIMEOUT);

    // One block was mined height should increase by one
    QCOMPARE(_walletA->bestHeight(), startingHeight + 1);

    lastCount = spy_blocks_synched.count();

    // Generate one more block
    QVERIFY(regtest::generate_blocks(1) == 0);

    // Wait to receive the new block
    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > lastCount, DEFAULT_SIGNAL_TIMEOUT);

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

    //_walletA->create(WALLET_SEED);
    _walletA->create();

    auto addr = _walletA->generateReceiveAddress();

    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    uint64_t startingConfirmedBalance = _walletA->balance();
    uint64_t startingUnconfirmedBalance = _walletA->unconfirmedBalance();

    int lastBalanceChangeCount = spy_balance_changed.count();

    // Send 0.005BTC to our wallet
    QVERIFY(regtest::send_to_address(addr.toBase58CheckEncoding().toStdString(), "0.005") == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > lastBalanceChangeCount, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(_walletA->balance(), startingConfirmedBalance);
    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(500000)));

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

    QCOMPARE(_walletA->balance(), uint64_t(startingConfirmedBalance + uint64_t(500000)) );
    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(startingUnconfirmedBalance + uint64_t(1000000)));

    // Simulate effect of a reorg

    int startAtHeight = _walletA->bestHeight() - 1;
    // Send another 0.003BTC to our wallet
    QVERIFY(regtest::send_to_address(addr.toBase58CheckEncoding().toStdString(), "0.003") == 0);
    QVERIFY(regtest::generate_blocks(3) == 0);

    _walletA->test_syncBlocksStaringAtHeight(startAtHeight);
    QTest::qWait(15000);

    QCOMPARE(_walletA->balance(), uint64_t(startingConfirmedBalance + uint64_t(1300000)) );

    _walletA->stopSync();

    // Make sure there were no store errors
    QCOMPARE(spy_store_error.count(), 0);
}

void Test::Utxo() {
    _walletA->create();

    auto addr1 = _walletA->generateReceiveAddress();
    auto addr2 = _walletA->generateReceiveAddress();
    auto addr3 = _walletA->generateReceiveAddress();

    // 100,000 satoshi (2 conf)
    QVERIFY(regtest::send_to_address(addr1.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    //  50,000 satoshi (1 conf)
    QVERIFY(regtest::send_to_address(addr2.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);


    QSignalSpy spy_blocks_synched(_walletA, SIGNAL(synched()));

    // Should connect and synch headers
    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(spy_blocks_synched.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changed(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    //  25,000 satoshi (0 conf)
    QVERIFY(regtest::send_to_address(addr3.toBase58CheckEncoding().toStdString(), "0.00025") == 0);

    // Wait for balance to change
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changed.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(175000));

    Coin::UnspentOutputSet lockedOutputs;

    // Check existence of all UTXOs
    {
        auto utxos(_walletA->lockOutputs(100000, 2));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(100000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr1.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    {
        auto utxos(_walletA->lockOutputs(50000, 1));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(50000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr2.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    // An empty utxo list should be returned when not enough funds available
    {
        auto utxos(_walletA->lockOutputs(100000, 0));
        QCOMPARE(int(utxos.size()), 0);
    }

    {
        auto utxos(_walletA->lockOutputs(25000, 0));
        QCOMPARE(int(utxos.size()), 1);
        QCOMPARE(uint64_t(utxos.value()), uint64_t(25000));
        QCOMPARE((*utxos.begin())->scriptPubKey().getHex(), Coin::P2PKHScriptPubKey(addr3.pubKeyHash()).serialize().getHex());
        lockedOutputs.insert(*utxos.begin());
    }

    // Outputs can be unlocked
    QCOMPARE(_walletA->unlockOutputs(lockedOutputs), uint(3));


    /* Largest Utxos are returned first
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
    */
}

void Test::BroadcastingTx() {
    _walletA->create();
    _walletB->create();

    auto addrA = _walletA->generateReceiveAddress();
    auto addrB = _walletB->generateReceiveAddress();

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(_walletA, SIGNAL(synched()));

    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy synchedB(_walletB, SIGNAL(synched()));

    _walletB->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_balance_changedB(_walletB, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    _walletA->test_sendToAddress(70000, addrB, 5000);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(_walletB->unconfirmedBalance(), uint64_t(70000));

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(25000));

}

void Test::UsingOptionalDataInP2SHSpend() {
    _walletA->create();

    // script = OP_ADD 2 OP_EQUALVERIFY
    uchar_vector script;
    script.push_back(0x93); script.push_back(0x52); script.push_back(0x88);

    // data:  OP_1 OP_1
    uchar_vector data;
    data.push_back(0x51); data.push_back(0x51);

    Coin::PrivateKey key(_walletA->generateKey([&script, &data](const Coin::PublicKey &pubkey){
        // Push Public Key to script
        script += Coin::opPushData(0x21);
        script += pubkey.toUCharVector();

        script.push_back(0xac); // OP_CHECKSIG

        return joystream::bitcoin::RedeemScriptInfo(script, data);
    }));

    Coin::P2SHAddress addrA = Coin::P2SHAddress(Coin::Network::regtest, Coin::RedeemScriptHash::fromRawScript(script));

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(_walletA, SIGNAL(synched()));

    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(100000));

    {
        auto utxos = _walletA->lockOutputs(0, 0);
        // Our custom p2sh output should not be recognised as a standard output
        QCOMPARE(uint(utxos.size()), uint(0));
        _walletA->unlockOutputs(utxos);
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
        auto utxos = _walletA->lockOutputs(0, 0, selector);
        QCOMPARE(uint(utxos.size()), uint(1));
        _walletA->unlockOutputs(utxos);
    }

    _walletA->test_sendToAddress(10000, addrA, 5000, selector);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(95000));
}

void Test::FinanceTxFromMultipleSets() {
    _walletA->create();
    _walletB->create();

    auto addrA = _walletA->generateReceiveAddress();
    auto addrB = _walletB->generateReceiveAddress();

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00050") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(_walletA, SIGNAL(synched()));

    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy synchedB(_walletB, SIGNAL(synched()));

    _walletB->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QSignalSpy spy_balance_changedA(_walletA, SIGNAL(balanceChanged(uint64_t, uint64_t)));
    QSignalSpy spy_balance_changedB(_walletB, SIGNAL(balanceChanged(uint64_t, uint64_t)));

    auto utxoSet1(_walletA->lockOutputs(50000));
    auto utxoSet2(_walletA->lockOutputs(50000));

    Coin::Transaction tx;

    tx.addOutput(Coin::TxOut(70000, Coin::P2PKHScriptPubKey(addrB.pubKeyHash()).serialize())); // to walletB
    tx.addOutput(Coin::TxOut(25000, Coin::P2PKHScriptPubKey(addrA.pubKeyHash()).serialize())); // change

    utxoSet1.finance(tx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, true));
    utxoSet2.finance(tx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, true));

    _walletA->broadcastTx(tx);

    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);
    QTRY_VERIFY_WITH_TIMEOUT(spy_balance_changedB.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    QCOMPARE(_walletB->unconfirmedBalance(), uint64_t(70000));

    QCOMPARE(_walletA->unconfirmedBalance(), uint64_t(25000));
}

void Test::RedeemScriptFiltering() {
    _walletA->create();

    // script = OP_ADD 2 OP_EQUALVERIFY
    uchar_vector script;
    script.push_back(0x93); script.push_back(0x52); script.push_back(0x88);

    Coin::PrivateKey key(_walletA->generateKey([&script](const Coin::PublicKey &pubkey){
        return script;
    }));

    Coin::P2SHAddress addrA = Coin::P2SHAddress(Coin::Network::regtest, Coin::RedeemScriptHash::fromRawScript(script));

    QVERIFY(regtest::send_to_address(addrA.toBase58CheckEncoding().toStdString(), "0.00100") == 0);
    QVERIFY(regtest::generate_blocks(1) == 0);

    QSignalSpy synchedA(_walletA, SIGNAL(synched()));

    _walletA->sync("localhost", 18444);

    QTRY_VERIFY_WITH_TIMEOUT(synchedA.count() > 0, DEFAULT_SIGNAL_TIMEOUT);

    auto selector1 = [&script](const joystream::bitcoin::Store::StoreControlledOutput &output) -> Coin::UnspentOutput* {
        return nullptr;
    };

    auto outputs1 = _walletA->lockOutputs(10000,0, selector1);

    QCOMPARE(uint(outputs1.size()), uint(0));


    auto selector2 = [&script](const joystream::bitcoin::Store::StoreControlledOutput &output) -> Coin::UnspentOutput* {

        if(script == output.redeemScript) {
            return new Coin::UnspentP2SHOutput(output.keyPair, output.redeemScript, output.optionalData, output.outPoint, output.value);
        }

        return nullptr;
    };

    auto outputs2 = _walletA->lockOutputs(10000,0, selector2);

    QCOMPARE(uint(outputs2.size()), uint(1));

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
