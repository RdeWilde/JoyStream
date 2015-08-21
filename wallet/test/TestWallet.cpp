#include "TestWallet.hpp"

#include <common/Network.hpp>
#include <common/TransactionId.hpp>
#include <common/Seed.hpp>
#include <common/Utilities.hpp> // Coin::networkToAddressVersions
#include <wallet/Key.hpp>
#include <wallet/Address.hpp>
#include <wallet/Manager.hpp>
#include <wallet/OutPoint.hpp>

#include <CoinCore/CoinNodeData.h>
#include <CoinQ/CoinQ_script.h>

void TestWallet::initTestCase() {

    // Delete any lingering wallet file
    QFile::remove(WALLET_FILE_NAME);

    // Use test seed to generate key chain
    Coin::Seed seed = Coin::Seed::testSeeds[0];

    // Create wallet
    Wallet::Manager::createNewWallet(WALLET_FILE_NAME, NETWORK_TYPE, seed);

    // Open the wallet
    _manager = new Wallet::Manager(WALLET_FILE_NAME);

    //
    _db = _manager->db();
}

/**
void TestWallet::outPoint_data() {

    QTest::addColumn<Coin::TransactionId>("txId");
    QTest::addColumn<int>("index");

    QTest::newRow("first")  << Coin::TransactionId() << 0;
    QTest::newRow("second") << Coin::TransactionId("1234") << 44;
}
*/

void TestWallet::outPoint() {

    Coin::OutPoint original(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 11);

    // add
    QVERIFY(_manager->addOutPoint(original));

    // fail to add twice
    QVERIFY(!_manager->addOutPoint(original));

    // check that its there
    QVERIFY(Wallet::OutPoint::exists(_db, Wallet::OutPoint::PK(original)));

    // check that different tx isnt there
    Coin::OutPoint other_tx(uchar_vector("6404f7cfc0cc00e4234123414125978d0c021edecad6e3f613b1575b7d7aa160"), 11);
    QVERIFY(!Wallet::OutPoint::exists(_db, Wallet::OutPoint::PK(other_tx)));

    // check that different index isnt there
    Coin::OutPoint other_index(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 666);
    QVERIFY(!Wallet::OutPoint::exists(_db, Wallet::OutPoint::PK(other_index)));

    // try to get it out, and that its the same
    Wallet::OutPoint::Record record;
    bool exists = Wallet::OutPoint::exists(_db, original, record);
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
    QVERIFY(Wallet::Key::exists(_db, 0, record));

    // Equal to inserted original
    QVERIFY(sk == record._privateKey);

    // Different from new random (all null actually) key
    QVERIFY(Coin::PrivateKey() != record._privateKey);

    // Nothing else has been inserted
    QVERIFY(!Wallet::Key::exists(_db, 1));
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
    int numberOfOutputs = 2;

    // Create transaction to add
    Coin::Transaction tx;

    for(int i = 0; i < numberOfOutputs;i++) {

        // Get receive address
        Coin::P2PKHAddress address = _manager->getReceiveAddress();

        // Generate output script
        uchar_vector scriptPubKey = CoinQ::Script::getTxOutScriptForAddress(address.toBase58CheckEncoding().toStdString(), Coin::networkToAddressVersions(NETWORK_TYPE));

        // Generate output
        Coin::TxOut out(i*3 + 1, scriptPubKey);

        // Add to transaction
        tx.addOutput(out);
    }

    // Add to wallet
    QVERIFY(_manager->addTransaction(tx));

    // Get utxo
    QList<Coin::UnspentP2PKHOutput> zeroConfUtxo = _manager->listUtxo(0);

    // Check that it is the right size
    QVERIFY(zeroConfUtxo.size() == numberOfOutputs);

    //
    // Add a transaction which spends one of the outputs
    //

    Coin::Transaction spendingTx;
    Coin::OutPoint out(tx.getHashLittleEndian(),0); // outpoint referencing first outputo tx above
    spendingTx.addInput(Coin::TxIn(out, uchar_vector("12345"), 4444));

    // Add to wallet
    QVERIFY(_manager->addTransaction(spendingTx));

    // Get utxo
    QList<Coin::UnspentP2PKHOutput> zeroConfUtxo2 = _manager->listUtxo(0);

    // Check that it is the right size
    QVERIFY(zeroConfUtxo2.size() == numberOfOutputs - 1);

}

/*
void TestWallet::input_data() {


    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello" << "HELLO";
    QTest::newRow("mixed")     << "Hello" << "HELLO";
    QTest::newRow("all upper") << "HELLO" << "HELLO";

}
*/

void TestWallet::cleanupTestCase() {

}

QTEST_MAIN(TestWallet)
#include "moc_TestWallet.cpp"
