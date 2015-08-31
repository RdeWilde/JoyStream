/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#include "Test.hpp"

#include <common/Utilities.hpp>
#include <CoinCore/secp256k1.h>

//#include <CoinCore/CoinNodeData.h>
//#include <CoinQ/CoinQ_script.h>


//#include <common/Seed.hpp>


void Test::makeAndCheckSig() {

    // Create fresh private keys
    CoinCrypto::secp256k1_key key1, key2, key3;
    key1.newKey();
    key2.newKey();
    key3.newKey();

    // Create

    // Create transaction
    Coin::Transaction tx;
    //tx.addOutput(Coin::TxOut(500, ));



    //




}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
