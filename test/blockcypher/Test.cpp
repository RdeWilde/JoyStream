/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>

#include <blockcypher/Client.hpp>
#include <blockcypher/Wallet.hpp>

#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinCore/hdkeys.h>

void Test::init() {
    _client = new BlockCypher::Client(&_manager, NETWORK_TYPE, BLOCKCYPHER_TOKEN);
}

void Test::cleanup() {
    delete _client;
}

void Test::createWallet() {

    // Create wallet to request
    QList<Coin::P2PKHAddress> list;
    list.append(Coin::P2PKHAddress());
    list.append(Coin::P2PKHAddress());

    // Request new wallet:
    // Havent found a sensible way to request new wallet without also
    // having to implement wallet deletion, so wait for now.
    //BlockCypher::Wallet requestedNew(BLOCKCYPHER_TOKEN, "my-test5", list);
    //BlockCypher::Wallet returned = _client->createWallet(requestedNew);
    //QVERIFY(returned == requestedNew);

    // Request wallet which already exists
    BlockCypher::Wallet requested(BLOCKCYPHER_TOKEN, "addAddresses-testwallet", list);
    QVERIFY_EXCEPTION_THROWN(_client->createWallet(requested), std::runtime_error);
}

void Test::getWallet() {

    // Check that existing wallet is found
    BlockCypher::Wallet returned = _client->getWallet("my-test");
    QVERIFY(returned._name == "my-test");

    // Check that non-existing wallet is not found
    QVERIFY_EXCEPTION_THROWN(_client->getWallet("unused-wallet-name"), std::runtime_error);
}

void Test::addAddresses() {

    // Get old wallet
    BlockCypher::Wallet init = _client->getWallet("addAddresses-testwallet");

    // Generate a new public key for which to add the corresponding address to the wallet
    Coin::P2PKHAddress addr(NETWORK_TYPE, Coin::PublicKey(Coin::PrivateKey::generate().toPublicKey()).toPubKeyHash());
    BlockCypher::Wallet toBeAdded(init._token, init._name, QList<Coin::P2PKHAddress>() << addr);

    // Add to wallet
    BlockCypher::Wallet final = _client->addAddressToWallet(toBeAdded);

    // Check that size increased by one
    QVERIFY(final._addresses.size() == init._addresses.size() + 1);
}

void Test::txref() {

}

void Test::wallet() {

    //BlockCypher::Wallet wallet();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
