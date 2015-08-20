#include "TestWallet.hpp"

#include <common/Network.hpp>
#include <common/TransactionId.hpp>
#include <common/Seed.hpp>
#include <wallet/Key.hpp>
#include <wallet/Address.hpp>
#include <wallet/Manager.hpp>
#include <wallet/OutPoint.hpp>

#include <CoinCore/CoinNodeData.h>


void TestWallet::initTestCase() {

    // Delete any lingering wallet file
    QFile::remove(WALLET_FILE_NAME);

    // Use test seed to generate key chain
    Coin::Seed seed = Coin::Seed::testSeeds[0];

    // Create wallet
    Wallet::Manager::createNewWallet(WALLET_FILE_NAME, Coin::Network::testnet3, seed);

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

    //tes
    Coin::Transaction tx;
    Coin::OutPoint outPoint(uchar_vector("6404f7cfc0cc00e402247c309345978d0c021edecad6e3f613b1575b7d7aa160"), 666);
    Coin::TxIn in(outPoint, uchar_vector("222"), 1234);
    tx.addInput(in);

    Coin::TxOut out(33, uchar_vector("1111111"));
    tx.addOutput(out);

    bool itworked = _manager->addTransaction(tx);

    QVERIFY(itworked);

    //qDebug() << QString::fromStdString(tx.getHashLittleEndian().getHex());

    Coin::Transaction tx2 = _manager->getTransaction(tx.getHashLittleEndian());

    QVERIFY(tx.getHashLittleEndian() == tx2.getHashLittleEndian());

    tx2.version = 0;

    QVERIFY(tx.getHashLittleEndian() != tx2.getHashLittleEndian());

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
