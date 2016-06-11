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
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2SHScriptSig.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/secp256k1_openssl.h>

#include <QJsonArray>

#include <QDebug>

void Test::transactionId() {

    // Transaction: https://live.blockcypher.com/bcy/tx/6550109a0be63c8ae647478be1320fa00ff9d19d509f18432723e160a694949d/
    Coin::Transaction tx("01000000010000000000000000000000000000000000000000000000000000000000000000000000001774657374373230373734333134313533393132383336390000000001807c814a00000000232102a44f60c94b840854db8c673e280dbc76b2975c6cf10e351ef6208f7f546e2130ac00000000");
    std::string txId("6550109a0be63c8ae647478be1320fa00ff9d19d509f18432723e160a694949d");

    // Parse transaction id
    Coin::TransactionId id = Coin::TransactionId::fromRPCByteOrder(txId);

    // Derive transaction id from transaction, and check that its
    Coin::TransactionId derivedId = Coin::TransactionId::fromTx(tx);
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
    QVERIFY(CoinCrypto::secp256k1_verify(signatureCheckingKey, message, sig, CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S));

    // Check false signature
    try {

        // Should throw
        bool hold = CoinCrypto::secp256k1_verify(signatureCheckingKey, message, bytes_t(sig.size()), CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S);

        QVERIFY(!hold);
    } catch (std::runtime_error & e) {
        QVERIFY(true);
    }

    // Check correct signature for incorrect message
    try {

        // Should throw or be invalid
        bool holds  = CoinCrypto::secp256k1_verify(signatureCheckingKey, uchar_vector(23, 0), sig, CoinCrypto::SignatureFlag::SIGNATURE_ENFORCE_LOW_S);

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
    expectedResult.reverse(); // turn into rpc??? byte order

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


void Test::popData() {

    {
        uchar_vector script;
        script.push_back(0x00); // OP_FALSE

        uchar_vector data;

        uchar_vector subscript = Coin::popData(script, data);

        QCOMPARE(uint(data.size()), uint(0));

        QVERIFY(subscript.empty());
    }

    {
        uchar_vector script;
        script.push_back(0x00); // OP_FALSE
        script.push_back(0x00); // OP_FALSE

        QCOMPARE(uint(script.size()), uint(2));

        uchar_vector data;

        uchar_vector subscript = Coin::popData(script, data);

        QCOMPARE(uint(data.size()), uint(0));

        QCOMPARE(uint(subscript.size()), uint(1));

        Coin::popData(subscript, data);

        QCOMPARE(uint(data.size()), uint(0));
    }

    {
        const uint len = 20;
        uchar_vector data(len, 0xff);
        uchar_vector script;
        script += Coin::opPushData(data.size());
        script += data;

        QCOMPARE(uint(script.size()), uint(21));
        QCOMPARE(uint(script[0]), len);

        uchar_vector data_;
        uchar_vector subscript = Coin::popData(script, data_);

        QCOMPARE(uint(data_.size()), uint(len));

        QCOMPARE(data_, data);

        QVERIFY(subscript.empty());
    }

    {
        const uint len = 20;
        uchar_vector data(len, 0xff);
        uchar_vector script;
        script += Coin::opPushData(data.size());
        script += data;
        script += Coin::opPushData(data.size());
        script += data;

        uchar_vector subscript;

        QCOMPARE(uint(script.size()), uint(42));

        uchar_vector data_;

        subscript = Coin::popData(script, data_);

        QCOMPARE(uint(data_.size()), uint(len));
        QCOMPARE(data_, data);

        QCOMPARE(uint(subscript.size()), uint(21));

        Coin::popData(subscript, data_);

        QCOMPARE(uint(data_.size()), uint(len));
        QCOMPARE(data_, data);
    }

}

void Test::P2PKHScriptPubKey() {

    /**
     * Bitcore script test data generator
    var compressedPK = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
    var scriptPubkey = bitcore.Script.buildPublicKeyHashOut(compressedPK);
    console.log(scriptPubkey);
    console.log(scriptPubkey.toBuffer());

    Output:
    <Script: OP_DUP OP_HASH160 20 0xcbf730e06e5f8e4fc44f071d436a4660ddde3e47 OP_EQUALVERIFY OP_CHECKSIG>
    <Buffer 76 a9 14 cb f7 30 e0 6e 5f 8e 4f c4 4f 07 1d 43 6a 46 60 dd de 3e 47 88 ac>
    */

    uchar_vector rawPk("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector serializedP2PKH("76a914cbf730e06e5f8e4fc44f071d436a4660ddde3e4788ac");

    Coin::PublicKey pk(rawPk);

    Coin::P2PKHScriptPubKey script(pk);

    QVERIFY(script.serialize() == serializedP2PKH);
}

void Test::P2PKHScriptSig() {

    /**
    // P2PKHScriptSig
    // NOTICE: PUBLIC KEY AND SIGNATURES DO NOT MATCH, ITS JUST FOR TESTING
    var compressedPK = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);

    var sigHexaStr = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
        '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
    var sig = Signature.fromCompact(new Buffer(sigHexaStr, 'hex'));

    console.log(sig.toBuffer().toString('hex'));

    var scriptSig = Script.buildPublicKeyHashIn(compressedPK, sig, bitcore.crypto.Signature.SIGHASH_ALL);
    console.log(scriptSig.toString());
    console.log(scriptSig.toBuffer().toString('hex'));

    Output:
    3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
    72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc
    483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc60121030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc
    */

    uchar_vector rawPk("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawSig("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    Coin::PublicKey pk(rawPk);
    Coin::TransactionSignature ts(Coin::Signature(rawSig), Coin::SigHashType::standard());
    Coin::P2PKHScriptSig sig(pk, ts);
    uchar_vector serializedP2PKHScriptSig("483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc60121030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");

    QVERIFY(sig.serialized() == serializedP2PKHScriptSig);

    Coin::P2PKHScriptSig deserializedScript(Coin::P2PKHScriptSig::deserialize(serializedP2PKHScriptSig));

    QVERIFY(deserializedScript.pk() == pk);
    QVERIFY(deserializedScript.ts() == ts);

    QVERIFY(deserializedScript.serialized() == serializedP2PKHScriptSig);
}

void Test::MultisigScriptPubKey() {

    /**
    // MultisigScriptPubKey
    var compressedPK_1 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
    var compressedPK_2 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc', net);

    var scriptPubKey = Script.buildMultisigOut([compressedPK_1, compressedPK_2], 2, {noSorting : true}); // remembe to not sort
    var redeemScriptHash = bitcore.crypto.Hash.sha256ripemd160(scriptPubKey.toBuffer());

    console.log(scriptPubKey.toString());
    console.log(scriptPubKey.toBuffer().toString('hex'));
    console.log(redeemScriptHash.toString('hex'));

    Output:
    OP_2 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc 33 0x030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc OP_2 OP_CHECKMULTISIG
    5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae
    4fbd6060861fed97ef2b95e6afff4afb2943cc15
    */

    uchar_vector rawPk_1("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawPk_2("030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc");
    uchar_vector rawScript("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
    //uchar_vector rawScriptHash("4fbd6060861fed97ef2b95e6afff4afb2943cc15");

    Coin::PublicKey pk1(rawPk_1);
    Coin::PublicKey pk2(rawPk_2);

    Coin::MultisigScriptPubKey script(std::vector<Coin::PublicKey>({pk1, pk2}), 2);

    QVERIFY(script.serialized() == rawScript);
    //QVERIFY(script.scriptHash().toUCharVector() == rawScriptHash);
}

/**
void Test::MultisigScriptSig() {

}
*/

void Test::P2SHScriptPubKey() {

    /*
    // P2SHScriptPubKey
    var redeemScript = bitcore.Script("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
    var scriptPubKey = bitcore.Script.buildScriptHashOut(redeemScript);

    console.log(redeemScript.toString())
    console.log(scriptPubKey.toString());
    console.log(scriptPubKey.toBuffer().toString('hex'));

    Output:
    OP_2 33 0x030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc 33 0x030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc OP_2 OP_CHECKMULTISIG
    OP_HASH160 20 0x4fbd6060861fed97ef2b95e6afff4afb2943cc15 OP_EQUAL
    a9144fbd6060861fed97ef2b95e6afff4afb2943cc1587
    */

    uchar_vector rawRedeemScript("5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d1111183142bd0dd1a17abc99b0dc52ae");
    uchar_vector rawScript("a9144fbd6060861fed97ef2b95e6afff4afb2943cc1587");

    Coin::P2SHScriptPubKey script = Coin::P2SHScriptPubKey::fromSerializedRedeemScript(rawRedeemScript);

    QVERIFY(script.serialize() == rawScript);
}

void Test::P2SHScriptSig() {

}

void Test::P2SHMultisigScriptPubKey() {
    //  MUST turn multisigscripubkey to script hash of type redeemscriphash,
    // as this is how
}

void Test::P2SHMultisigScriptSig() {

    /**
    // MultisigScriptSig
    // NOTICE: PUBLIC KEY AND SIGNATURES DO NOT MATCH, ITS JUST FOR TESTING
    var compressedPK_1 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);
    var compressedPK_2 = bitcore.PublicKey('030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc', net);

    var sigHexaStr_1 = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
        '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
    var sig_1 = Signature.fromCompact(new Buffer(sigHexaStr_1, 'hex'));

    // buildP2SHMultisigIn needs sighash byte added to each signature already
    var sig_1_buffer = bitcore.util.buffer.concat([
        sig_1.toDER(),
        bitcore.util.buffer.integerAsSingleByteBuffer(bitcore.crypto.Signature.SIGHASH_ALL)
    ]);


    var sigHexaStr_2 = '1cd5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec12fc1188e8b' +
        '0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6';
    var sig_2 = Signature.fromCompact(new Buffer(sigHexaStr_2, 'hex'));
    var sig_2_buffer = bitcore.util.buffer.concat([
        sig_2.toDER(),
        bitcore.util.buffer.integerAsSingleByteBuffer(bitcore.crypto.Signature.SIGHASH_ALL)
    ]);

    var scriptSig = Script.buildP2SHMultisigIn([compressedPK_1, compressedPK_2], 2, [sig_1_buffer, sig_2_buffer], {noSorting : true});

    console.log(sig_1.toString());
    console.log(sig_2.toString());
    console.log(scriptSig.toString());
    console.log(scriptSig.toBuffer().toString('hex'));

    Output:
    3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
    3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6
    OP_0 72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 72 0x3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601 71 0x5221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae
    00483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601475221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae
    */

    uchar_vector rawPk_1("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawPk_2("030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc");
    uchar_vector rawSig_1("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    uchar_vector rawSig_2("3045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc6");
    uchar_vector script("00483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601483045022100d5e61ab5bfd0d1450997894cb1a53e917f89d82eb43f06fa96f32c96e061aec102202fc1188e8b0dc553a2588be2b5b68dbbd7f092894aa3397786e9c769c5348dc601475221030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc21030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc52ae");

    Coin::TransactionSignature ts_1(Coin::Signature(rawSig_1), Coin::SigHashType::standard());
    Coin::TransactionSignature ts_2(Coin::Signature(rawSig_2), Coin::SigHashType::standard());

    Coin::PublicKey pk_1(rawPk_1);
    Coin::PublicKey pk_2(rawPk_2);

    Coin::MultisigScriptPubKey redeemScript(std::vector<Coin::PublicKey>{pk_1, pk_2}, 2);
    Coin::P2SHScriptSig sig(std::vector<Coin::TransactionSignature>{ts_1, ts_2}, redeemScript.serialized());

    uchar_vector ser = sig.serialized();
    //qDebug() << QString::fromStdString(ser.getHex());
    QVERIFY(script == sig.serialized());

}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
