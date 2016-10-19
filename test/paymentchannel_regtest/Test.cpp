/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <Test.hpp>

#include "bitcoin_regtest_framework.hpp"

#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/UnspentOutput.hpp>
#include <common/UnspentOutputSet.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <boost/filesystem.hpp>
#include <boost/asio/impl/src.hpp>

using namespace joystream::test::bitcoin;

#define DEFAULT_SIGNAL_TIMEOUT 20 * 1000

void Test::initTestCase() {

    QVERIFY(regtest::init() == 0);
}

void Test::init() {

}

void Test::cleanup() {

}

void Test::cleanupTestCase() {
    // Stop bitcoind
    regtest::shutdown();
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
