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
        _wallet->Create(WALLET_SEED);
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

void Test::fundWallet()
{
    //_wallet->Create(WALLET_SEED);
    //_wallet->Open();
    //Coin::P2PKHAddress addr(_wallet->GetReceiveAddress());
    //QVERIFY(_client->fundWalletFromFaucet(addr.toBase58CheckEncoding(), 200));
}

void Test::getUtxo()
{
    _wallet->Create(WALLET_SEED);
    _wallet->Open();

    Coin::P2PKHAddress addr(_wallet->GetReceiveAddress());

    QVERIFY(_wallet->Sync());

    // Paid this address on Jan 19 2016 with two transactions - each sending 100 satoshi
    // Paid this address on Jan 20 2016 with one transaction - sending 200 satoshi
    // https://live.blockcypher.com/btc-testnet/address/mwqoYY153cUiQezsQiFAacC78rdDFUVJQF/

    // requesting 100 min satoshi should require only one of the outputs
    // It should return the minimum number of coins
    std::list<Coin::UnspentP2PKHOutput> outputs(_wallet->GetUnspentOutputs(200,0,0));
    QVERIFY(outputs.size() == 1);

    Coin::UnspentP2PKHOutput utxo1 = *outputs.begin();
    QVERIFY(utxo1.value() == 200);

    // on second request we should get two utxos
    std::list<Coin::UnspentP2PKHOutput> outputs2(_wallet->GetUnspentOutputs(200,0,0));
    QVERIFY(outputs2.size() == 2);

    Coin::UnspentP2PKHOutput utxo2 = *outputs2.begin();
    Coin::UnspentP2PKHOutput utxo3 = *(outputs2.begin()++);

    QVERIFY(utxo2.value() == 100);
    QVERIFY(utxo3.value() == 100);

    // no more utxos
    std::list<Coin::UnspentP2PKHOutput> outputs3(_wallet->GetUnspentOutputs(100,0,0));
    QVERIFY(outputs3.size() == 0);
}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
