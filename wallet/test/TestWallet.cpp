#include "TestWallet.hpp"
#include <common/Seed.hpp>
#include <common/Network.hpp>
#include <common/TransactionId.hpp>
#include <wallet/Manager.hpp>
#include <wallet/OutPoint.hpp>

#include <CoinCore/CoinNodeData.h>


void TestWallet::initTestCase() {

    // Delete any lingering wallet file
    QFile::remove(WALLET_FILE_NAME);

    // Create wallet
    Coin::Seed seed = Coin::Seed::testSeeds[0];
    Wallet::Manager::createNewWallet(WALLET_FILE_NAME, Coin::Network::testnet3, seed);

    // Open the wallet
    _manager = new Wallet::Manager(WALLET_FILE_NAME);

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

    QSqlDatabase db = _manager->db();

    Coin::OutPoint original(uchar_vector("111111"), 11);

    // add
    QVERIFY(_manager->addOutPoint(original));

    // check that its there
    QVERIFY(Wallet::OutPoint::exists(db, Wallet::OutPoint::PK(original)));

    // check that different tx isnt there
    Coin::OutPoint other_tx(uchar_vector("110111"), 11);
    QVERIFY(!Wallet::OutPoint::exists(db, Wallet::OutPoint::PK(other_tx)));

    // check that different index isnt there
    Coin::OutPoint other_index(uchar_vector("110111"), 11);
    QVERIFY(!Wallet::OutPoint::exists(db, Wallet::OutPoint::PK(other_index)));

    // try to get it out

}



void TestWallet::input() {


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
