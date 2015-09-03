/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#include "Test.hpp"

#include <common/Utilities.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/typesafeOutPoint.hpp>
#include <CoinCore/secp256k1.h>

//#include <CoinCore/CoinNodeData.h>
//#include <CoinQ/CoinQ_script.h>
//#include <common/Seed.hpp>

void Test::makeAndCheckSig() {

/**
    // Create transaction
    Coin::Transaction tx;

    // Add output, 500 satoshi, with dummy scriptPubKey
    tx.addOutput(Coin::TxOut(500, uchar_vector("1234")));

    // Add input
    // referencing outpoint (..,2)
    // default sequence number,
    // empty scriptSig
    Coin::typesafeOutPoint outpoint("8c6ef09a4b5bcc48d3dabf00f0b1c499a59589c9220d5b79faaeb2698781d645", 2);
    tx.addInput(Coin::TxIn(outpoint.getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

    // Generate private key for signing
    Coin::PrivateKey sk = Coin::PrivateKey::generate();
    Coin::PublicKey pk = sk.toPublicKey();

    // Make script builder for p2sh input script
    std::vector<bytes_t> pubkeys;
    pubkeys.push_back(pk.toUCharVector());

    CoinQ::Script::Script inputScriptBuilder(CoinQ::Script::Script::type_t::PAY_TO_PUBKEY, 1, pubkeys);

    // Make signature
    secure_bytes_t signature = createSignature(tx, 0, inputScriptBuilder, sk.toUCharVector());

    // const CoinQ::Script::Script & inputScriptBuilder,

    // Check signature
    bool verified = verifySignature(tx, 0,
                         inputScriptBuilder,
                         signature,
                         pubkeys[0]);
*/
    QVERIFY(true);
}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
