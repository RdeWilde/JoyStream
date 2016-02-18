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
#include <blockcypher/WebSocketClient.hpp>

#include <bitcoin/BlockCypherWallet.hpp>

#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <boost/filesystem.hpp>

void Test::init() {

    _client = new BlockCypher::Client(&_manager,
                                      TEST_BITCOIN_NETWORK,
                                      TEST_BLOCKCYPHER_TOKEN);

    _wsClient = new BlockCypher::WebSocketClient(TEST_BITCOIN_NETWORK, TEST_BLOCKCYPHER_TOKEN);

    // delete existing wallet file
    boost::filesystem::remove(TEST_WALLET_PATH);

    _wallet = new joystream::bitcoin::BlockCypherWallet(TEST_WALLET_PATH, TEST_BITCOIN_NETWORK, _client, _wsClient);
}

void Test::cleanup() {
    delete _client;

    _wsClient->disconnect();
    delete _wsClient;
}

void Test::walletCreation() {

    // Should create a fresh new wallet
    try{
        _wallet->Create();
        QVERIFY(true);
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Wallet file should now exist
    QVERIFY(boost::filesystem::exists(TEST_WALLET_PATH));

    // Should throw exception if we try to create wallet over existing file
    QVERIFY_EXCEPTION_THROWN(_wallet->Create(), std::exception);

    // Should open existing wallet
    try {
        _wallet->Open();
    } catch(std::exception & e) {
        QVERIFY(false);
    }

    // Wallet with no addresses syncs immediately
    QVERIFY(_wallet->Sync());
}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
