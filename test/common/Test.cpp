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

//#include <common/typesafeOutPoint.hpp>
//#include <CoinCore/CoinNodeData.h>
//#include <common/Seed.hpp>

#include <CoinCore/secp256k1.h>

#include <QJsonArray>

void Test::transactionId() {

    std::string txIdLiteral = "c5cbe43f771dd3f495a8257c7aac199ce3956c41a780ddb2a9a1cf8dced8a4f9";
    uchar_vector txIdUcharVector(txIdLiteral);

    // Check equality, and tohex routines, based on std::string ctr
    Coin::TransactionId txId(txIdUcharVector);
    QVERIFY(txIdUcharVector == txId.toUCharVector());
    QVERIFY(QString::fromStdString(txIdUcharVector.getHex()) == txId.toHex());

    // Same check, now from string based ctr
    Coin::TransactionId txIdFromString(txIdLiteral);
    QVERIFY(txIdUcharVector == txIdFromString.toUCharVector());
    QVERIFY(QString::fromStdString(txIdUcharVector.getHex()) == txIdFromString.toHex());

    /**
    std::string hxString = vector.getHex();
    QString hxQString = toHex();
    Q_ASSERT(QString::fromStdString(hxString) == hxQString);
    */
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

    // Create transactions to sign
    Coin::Transaction testTx("01000000031f5c38dfcf6f1a5f5a87c416076d392c87e6d41970d5ad5e477a02d66bde97580000000000ffffffff7cca453133921c50d5025878f7f738d1df891fd359763331935784cf6b9c82bf1200000000fffffffffccd319e04a996c96cfc0bf4c07539aa90bd0b1a700ef72fae535d6504f9a6220100000000ffffffff0280a81201000000001976a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac0084d717000000001976a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac00000000");

    // Create signing key
    Coin::PrivateKey sk = Coin::PrivateKey::generate();

    // Generate two signatures,
    Coin::TransactionSignature ts1 = sk.signForP2PKHSpend(testTx, 0);
    Coin::TransactionSignature ts2 = sk.signForP2PKHSpend(testTx, 0);

    // Check that they are identical: had some issues with this before
    //QVERIFY(ts1 == ts2);
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

    // Compute sighash and compare to correct answer
    uchar_vector actualResult(Coin::sighash(tx, input, scriptPubKey, type));

    // Compare :OBS for whateverr eason the byte strings are rerevsersd here!!! look into alter
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
