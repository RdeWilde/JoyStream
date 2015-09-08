/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <Test.hpp>

//#include <CoinCore/CoinNodeData.h>
//#include <QSqlDatabase>
//#define DB _manager->db()

void Test::init() {

    /**

    // Delete any lingering wallet file
    QFile::remove(WALLET_FILE_NAME);

    // Use test seed to generate key chain
    Coin::Seed seed = WALLET_SEED;

    // Create wallet
    Wallet::Manager::createNewWallet(WALLET_FILE_NAME, NETWORK_TYPE, seed);

    // Open the wallet
    _manager = new Wallet::Manager(WALLET_FILE_NAME);
    */

}

void Test::cleanup() {

    //delete _manager;
    //_manager = NULL;
}

/**
void TestWallet::outPoint_data() {

    QTest::addColumn<Coin::TransactionId>("txId");
    QTest::addColumn<int>("index");

    QTest::newRow("first")  << Coin::TransactionId() << 0;
    QTest::newRow("second") << Coin::TransactionId("1234") << 44;
}
*/

/**
void TestWallet::outPoint() {

    Coin::OutPoint original(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 11);

    // add
    QVERIFY(_manager->addOutPoint(original));

    // fail to add twice
    QVERIFY(!_manager->addOutPoint(original));

    // check that its there
    QVERIFY(Wallet::OutPoint::exists(DB, Wallet::OutPoint::PK(original)));

    // check that different tx isnt there
    Coin::OutPoint other_tx(uchar_vector("6404f7cfc0cc00e4234123414125978d0c021edecad6e3f613b1575b7d7aa160"), 11);
    QVERIFY(!Wallet::OutPoint::exists(DB, Wallet::OutPoint::PK(other_tx)));

    // check that different index isnt there
    Coin::OutPoint other_index(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 666);
    QVERIFY(!Wallet::OutPoint::exists(DB, Wallet::OutPoint::PK(other_index)));

    // try to get it out, and that its the same
    Wallet::OutPoint::Record record;
    bool exists = Wallet::OutPoint::exists(DB, original, record);
    QVERIFY(exists);

    // No comparison operator!!
    //QVERIFY(record.toOutPoint() == original);
    QVERIFY(record.toOutPoint().getSerialized() == original.getSerialized());

}

void TestWallet::input() {

}

void TestWallet::key() {

    //Coin::PrivateKey sk(_keyChain.getPrivateSigningKey(0));

    // Generate fresh key from wallet
    Coin::PrivateKey sk = _manager->issueKey();

    // Check that it has been inserted
    Wallet::Key::Record record;
    bool exists = Wallet::Key::exists(DB, 0, record);
    QVERIFY(exists);

    // Equal to inserted original
    QVERIFY(sk == record._privateKey);

    // Different from new random (all null actually) key
    QVERIFY(Coin::PrivateKey() != record._privateKey);

    // Nothing else has been inserted
    QVERIFY(!Wallet::Key::exists(DB, 1));
}

void TestWallet::address() {

}

void TestWallet::tx() {

    // Create transaction
    Coin::Transaction tx;

    Coin::TxIn in(Coin::OutPoint(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 666),
                  uchar_vector("222"),
                  1234);
    tx.addInput(in);

    Coin::TxOut out(33, uchar_vector("1111111"));
    tx.addOutput(out);

    // Add to wallet
    QVERIFY(_manager->addTransaction(tx));

    // Grab from wallet and check that it is the same
    Coin::Transaction tx2 = _manager->getTransaction(tx.getHashLittleEndian());
    QVERIFY(tx.getHashLittleEndian() == tx2.getHashLittleEndian());

    // Alter it and check that it's different
    tx2.version = 0;
    QVERIFY(tx.getHashLittleEndian() != tx2.getHashLittleEndian());

}

void TestWallet::listutxo() {

    // How many outputs
    quint32 numberOfOutputs = 2;

    // Create transaction
    Coin::Transaction tx = createWalletTx(numberOfOutputs);

    // Add to wallet
    QVERIFY(_manager->addTransaction(tx));

    // Check that it is the right size
    QList<Coin::UnspentP2PKHOutput> zeroConfUtxo_1 = _manager->listUtxo(0);
    QVERIFY(zeroConfUtxo_1.size() == numberOfOutputs);

    // Check that there are numberOfOutputs utxo created events
    QList<Wallet::UtxoCreated> utxoCreated_1 = _manager->getAllUtxoCreated(0);
    QVERIFY(utxoCreated_1.size() == numberOfOutputs);

    // Check that there are 0 utxo destroyed events
    QList<Wallet::UtxoDestroyed> utxoDestroyed_1 = _manager->getAllUtxoDestroyed(0);
    QVERIFY(utxoDestroyed_1.size() == 0);

    //
    // Add a transaction which spends one of the outputs
    //

    Coin::Transaction spendingTx;
    Coin::OutPoint out(tx.getHashLittleEndian(),0); // outpoint referencing first outputo tx above
    spendingTx.addInput(Coin::TxIn(out, uchar_vector("12345"), 4444));

    // Add to wallet
    QVERIFY(_manager->addTransaction(spendingTx));

    // Get utxo
    QList<Coin::UnspentP2PKHOutput> zeroConfUtxo_2 = _manager->listUtxo(0);
    QList<Wallet::UtxoCreated> utxoCreated_2 = _manager->getAllUtxoCreated(0);

    // Check that it is the right size
    QVERIFY(zeroConfUtxo_2.size() == numberOfOutputs - 1);
    QVERIFY(utxoCreated_2.size() == numberOfOutputs); // still same number of utxo created

    // Check that there are 1 utxo destroyed events
    QList<Wallet::UtxoDestroyed> utxoDestroyed_2 = _manager->getAllUtxoDestroyed(0);
    QVERIFY(utxoDestroyed_2.size() == 1);

}

void TestWallet::lockutxo() {

    // How many outputs
    quint32 numberOfOutputs = 5;

    // Create transaction
    Coin::Transaction tx = createWalletTx(numberOfOutputs);

    // Add to wallet
    QVERIFY(_manager->addTransaction(tx));

    // lock something we should be able to log
    quint64 quantityToLock = 8;
    QList<Coin::UnspentP2PKHOutput> list = _manager->lockUtxo(quantityToLock, 0);
    QVERIFY(!list.isEmpty());

    // count up how much we locked up
    quint64 totalLocked = 0;
    for(QList<Coin::UnspentP2PKHOutput>::const_iterator i = list.constBegin(),
        end = list.constEnd();
        i != end;
        i++) {

        Coin::UnspentP2PKHOutput output = *i;
        totalLocked += output.value();
    }

    QVERIFY(totalLocked >= quantityToLock);

    // Fail to lock a huge amount
    QVERIFY(_manager->lockUtxo(100000, 0).isEmpty());

    // Release, and see utxo grow back
    quint32 remainingUtxoSize = _manager->listUtxo(0).size();

    for(QList<Coin::UnspentP2PKHOutput>::const_iterator i = list.constBegin(),
        end = list.constEnd();
        i != end;
        i++) {

        // release
        Coin::UnspentP2PKHOutput output = *i;

        QVERIFY(_manager->releaseUtxo(output.outPoint()));
    }

    // We need some better test here that everyhthing was indeed released

    // Check we are back at original size
    //QVERIFY(oldUtxoSize + numberOfOutputs = );
}

Coin::Transaction TestWallet::createWalletTx(quint32 numberOfOutputs) {

    // Create transaction to add
    Coin::Transaction tx;

    for(int i = 0; i < numberOfOutputs;i++) {

        // Get receive address
        Coin::P2PKHAddress address = _manager->getReceiveAddress();

        // Generate output
        Coin::TxOut out(i*3 + 1, Coin::P2PKHScriptPubKey(address.pubKeyHash()).serialize());

        // Add to transaction
        tx.addOutput(out);
    }

    return tx;
}
*/

/*
void TestWallet::input_data() {


    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello" << "HELLO";
    QTest::newRow("mixed")     << "Hello" << "HELLO";
    QTest::newRow("all upper") << "HELLO" << "HELLO";

}
*/

QTEST_MAIN(Test)
#include "moc_Test.cpp"
