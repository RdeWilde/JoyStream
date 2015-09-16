/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#include <Test.hpp>

#include <common/TransactionId.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/Signature.hpp>
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/secp256k1.h>

#include <QJsonArray>

void Test::transactionId() {

    // Transaction: https://live.blockcypher.com/bcy/tx/6550109a0be63c8ae647478be1320fa00ff9d19d509f18432723e160a694949d/
    Coin::Transaction tx("01000000010000000000000000000000000000000000000000000000000000000000000000000000001774657374373230373734333134313533393132383336390000000001807c814a00000000232102a44f60c94b840854db8c673e280dbc76b2975c6cf10e351ef6208f7f546e2130ac00000000");
    std::string txId("6550109a0be63c8ae647478be1320fa00ff9d19d509f18432723e160a694949d");

    // Parse transaction id
    Coin::TransactionId id = Coin::TransactionId::fromRPCByteOrder(txId);

    // Derive transaction id from transaction, and check that its
    Coin::TransactionId derivedId(tx);
    QVERIFY(derivedId == id);

    // Check
    QVERIFY(txId == id.toRPCByteOrder());
}

void Test::basic() {

    // Create fresh private keys
    CoinCrypto::secp256k1_key key;
    key.newKey();

    // Message to be signed
    uchar_vector message(100, 55);

    // Compute signature
    bytes_t sig = CoinCrypto::secp256k1_sign(key, message);

    // Create signature checking key for this public key
    CoinCrypto::secp256k1_key signatureCheckingKey;
    signatureCheckingKey.setPubKey(key.getPubKey());

    // Check valid signature
    QVERIFY(CoinCrypto::secp256k1_verify(signatureCheckingKey, message, sig));

    // Check false signature
    try {

        // Should throw
        bool hold = CoinCrypto::secp256k1_verify(signatureCheckingKey, message, bytes_t(sig.size()));

        QVERIFY(!hold);
    } catch (std::runtime_error & e) {
        QVERIFY(true);
    }

    // Check correct signature for incorrect message
    try {

        // Should throw or be invalid
        bool holds  = CoinCrypto::secp256k1_verify(signatureCheckingKey, uchar_vector(23, 0), sig);

        QVERIFY(!holds);
    } catch (const std::runtime_error & e) {
        QVERIFY(true);
    }
}

void Test::makeAndCheckSig() {

    // Generate a new private key
    Coin::PrivateKey sk = Coin::PrivateKey::generate();

    // Get corresponding public key
    Coin::PublicKey pk = sk.toPublicKey();

    // Message to be signed
    uchar_vector message(100, 55);

    // Sign message
    Coin::Signature sig = sk.sign(message);

    // Check that valid signature works
    // Should never throw, since its valid
    QVERIFY(pk.verify(message, sig));

    // Check that false (blank) signature does not work
    try {

        // may throw
        bool valid = pk.verify(message, Coin::Signature());

        QVERIFY(!valid);

    } catch (const std::runtime_error & e) {
        QVERIFY(true);
    }

    // Check that valid signature does not work with incorrect public key
    try {

        Coin::PrivateKey skNew = Coin::PrivateKey::generate();

        bool valid = skNew.toPublicKey().verify(message, sig);

        QVERIFY(!valid);

    } catch (const std::runtime_error & e) {
        QVERIFY(true);
    }
}

void Test::sighash() {

    /**
     * WARNING, HE STANDARD DATA FROM BITCONI TESTING
     * HAS OP_CODESEPARATORS, SIGHASH FLAGS WE DONT SUPPORT,
     * AND INFACT INVALID NHASTHTYPES (wumpus said it
     * was to test compatibility with some old sighash routine,
     * bur agreed that they could not be used on net).
     */

    // raw tx from : https://github.com/bitcoin/bitcoin/blob/master/src/test/data/txcreate1.hex
    // ["raw_transaction, script, input_index, hashType, signature_hash (result)"]
    QJsonArray array { "01000000031f5c38dfcf6f1a5f5a87c416076d392c87e6d41970d5ad5e477a02d66bde97580000000000ffffffff7cca453133921c50d5025878f7f738d1df891fd359763331935784cf6b9c82bf1200000000fffffffffccd319e04a996c96cfc0bf4c07539aa90bd0b1a700ef72fae535d6504f9a6220100000000ffffffff0280a81201000000001976a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac0084d717000000001976a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac00000000",
                       "",
                       0,
                       1,
                       "bb7dcf9d789aa7288976eb83937436cea146878f72f4e36747ff5825008c20d6"};

    QVERIFY(sighash(array));
}

void Test::signForP2PKHSpend() {
}

bool Test::sighash(const QJsonArray & fixture) {

    // Parse transaction
    Coin::Transaction tx(fixture.at(0).toString().toStdString());

    // Parse scriptPubKey
    std::string hexTx = fixture.at(1).toString().toStdString();
    uchar_vector scriptPubKey(hexTx);

    // Parse input
    uint input = fixture.at(2).toInt();

    // Parse hash code
    uint32_t hashCode = fixture.at(3).toInt();

    Coin::SigHashType type;

    try {
        type = Coin::SigHashType::fromHashCode(hashCode);
    } catch (const std::runtime_error & e) {
        // If no other flag is set, we just go with SIGHASH_ALL and anyone cant pay... Bitcoin madness
        type = Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, Coin::SigHashType::canAnyonePay(hashCode));
    }

    // non standard hash type
    if(!type.isStandard())
        return true;

    // Parse resulting correct sighash
    uchar_vector expectedResult(fixture.at(4).toString().toStdString());
    expectedResult.reverse(); // turn into internal byte order

    // Compute sighash and compare to correct answer
    uchar_vector actualResult(Coin::sighash(tx, input, scriptPubKey, type));

    // Compare :OBS for whateverr eason the byte strings are rerevsersd here!!! look into alter
    // I THINK IT IS DUE TO THE FACT THAT THIS RAW DATA WAS PRINTED OUT IN BLOCKEXPLORER/BIG ENDIAN MODE,
    // WHILE RAW DATA IS LITTLE ENDIAN.
    return expectedResult == actualResult;
}

void Test::addresses() {

    // pubkeyhas

    // p2pkh

    // p2sh
}

void Test::scripts() {

    // p2pkh out

    // p2pkh in

    // mofn out

    // mofn in

    // p2sh out

    // p2sh in

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
