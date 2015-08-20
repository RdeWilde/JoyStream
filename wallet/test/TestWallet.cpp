#include "TestWallet.hpp"
#include <common/Seed.hpp>
#include <common/Network.hpp>
#include <wallet/Manager.hpp>


void TestWallet::initTestCase() {

    // Delete any lingering wallet file
    QFile::remove(WALLET_FILE_NAME);

    // Create wallet
    Coin::Seed seed = Coin::Seed::testSeeds[0];
    Wallet::Manager::createNewWallet(WALLET_FILE_NAME, Coin::Network::testnet3, seed);

    // Open the wallet


}

void TestWallet::inputs() {

}

void TestWallet::cleanupTestCase() {

}

QTEST_MAIN(TestWallet)
//#include ""
