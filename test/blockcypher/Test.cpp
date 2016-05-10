/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>

#include <blockcypher/Client.hpp>
#include <blockcypher/Wallet.hpp>
#include <blockcypher/Address.hpp>

#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

void Test::init() {

    _client = new blockcypher::Client(&_manager,
                                      TEST_BITCOIN_NETWORK,
                                      TEST_BLOCKCYPHER_TOKEN);
}

void Test::cleanup() {
    delete _client;
}

void Test::createWallet() {

    // Create address list for wallet, must be at least one address
    QList<Coin::P2PKHAddress> list = { Coin::P2PKHAddress() };

    // Random wallet name to be created
    // ** ugly **
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QString walletName = "createWallet-" + QString::number(qrand());
    walletName = walletName.left(25); // take leftmost min(25, walletName.size()) characters

    // Create wallet:
    blockcypher::Wallet newWallet(TEST_BLOCKCYPHER_TOKEN, walletName, list);
    blockcypher::Wallet returnedWallet = _client->createWallet(newWallet);
    QVERIFY(returnedWallet == newWallet);

    // Try to create same wallet again
    blockcypher::Wallet requested(TEST_BLOCKCYPHER_TOKEN, walletName, list);
    QVERIFY_EXCEPTION_THROWN(_client->createWallet(requested), std::runtime_error);
}

void Test::deleteWallet() {

    // Create wallet
    QList<Coin::P2PKHAddress> list = { Coin::P2PKHAddress() };
    blockcypher::Wallet newWallet(TEST_BLOCKCYPHER_TOKEN, "temp-wallet", list);
    blockcypher::Wallet returnedWallet = _client->createWallet(newWallet);
    QVERIFY(returnedWallet == newWallet);

    // Delete wallet
    _client->deleteWallet("temp-wallet");

    // Check that getting it throws and exception,
    // i.e. it does not work
}

void Test::getWallet() {

    try {
        // Try to create wallet, will fail since it mostly exists,
        // but we need to try to ensure
        QList<Coin::P2PKHAddress> list = { Coin::P2PKHAddress() };
        blockcypher::Wallet newWallet(TEST_BLOCKCYPHER_TOKEN, "my-test", list);
        _client->createWallet(newWallet);
    } catch (const std::runtime_error & e) {
    }

    // Check that existing wallet is found
    blockcypher::Wallet returned = _client->getWallet("my-test");
    QVERIFY(returned._name == "my-test");

    // Check that non-existing wallet is not found
    QVERIFY_EXCEPTION_THROWN(_client->getWallet("unused-wallet-name"), std::runtime_error);
}

void Test::addAddresses() {

    // Get old wallet
    blockcypher::Wallet init = _client->getWallet("my-test");

    // Generate a new public key for which to add the corresponding address to the wallet
    Coin::P2PKHAddress addr(TEST_BITCOIN_NETWORK, Coin::PublicKey(Coin::PrivateKey::generate().toPublicKey()).toPubKeyHash());
    blockcypher::Wallet toBeAdded(init._token, init._name, QList<Coin::P2PKHAddress>() << addr);

    // Add to wallet
    blockcypher::Wallet final = _client->addAddressToWallet(toBeAdded);

    // Check that size increased by one
    QVERIFY(final._addresses.size() == init._addresses.size() + 1);
}

void Test::addressEndpoint() {

    blockcypher::Address addr = *_client->addressEndPoint("my-test").begin();

    QVERIFY(addr._balance > 0);
}

void Test::pushRawTransaction() {

    Coin::Transaction test("01000000011935b41d12936df99d322ac8972b74ecff7b79408bbccaf1b2eb8015228beac8000000006b483045022100921fc36b911094280f07d8504a80fbab9b823a25f102e2bc69b14bcd369dfc7902200d07067d47f040e724b556e5bc3061af132d5a47bd96e901429d53c41e0f8cca012102152e2bb5b273561ece7bbe8b1df51a4c44f5ab0bc940c105045e2cc77e618044ffffffff0240420f00000000001976a9145fb1af31edd2aa5a2bbaa24f6043d6ec31f7e63288ac20da3c00000000001976a914efec6de6c253e657a9d5506a78ee48d89762fb3188ac00000000");

    //bool broadcasted = _client->pushRawTransaction(test);

    //QVERIFY(broadcasted);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
