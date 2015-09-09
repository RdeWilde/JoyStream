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

    BlockCypher::Wallet requested(BLOCKCYPHER_TOKEN, "my-test4", list);

    BlockCypher::Wallet returned = _client->createWallet(requested);

    QVERIFY(returned == requested);
}

void Test::address() {

    // create wallet
}

void Test::txref() {

}

void Test::wallet() {

    //BlockCypher::Wallet wallet();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
