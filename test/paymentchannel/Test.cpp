/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <Test.hpp>

#include <common/Seed.hpp>
#include <common/Payment.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/TransactionSignature.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/P2SHAddress.hpp>
#include <common/RedeemScriptHash.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <CoinCore/CoinNodeData.h> // Transaction
#include <CoinCore/hdkeys.h>

#include <ctime>

using namespace joystream::paymentchannel;

void Test::redeemScript() {

    Coin::KeyPair payorPair = Coin::KeyPair::generate();

    Coin::KeyPair payeePair = Coin::KeyPair::generate();

    uint32_t lockTime = std::time(nullptr);

    RedeemScript rs(payorPair.pk(), payeePair.pk(), lockTime);

    uchar_vector serialized = rs.serialized();

    RedeemScript rs_des = RedeemScript::deserialize(serialized);

    QCOMPARE(serialized, rs_des.serialized());
}

void Test::refund() {

    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();

    Coin::typesafeOutPoint contractOutPoint;

    // settlement final destinations
    Coin::P2PKScriptPubKey payorScriptPubKey(payorFinalPair.pk());
    Coin::RedeemScriptHash payorScriptHash(payorScriptPubKey);

    Coin::P2PKScriptPubKey payeeScriptPubKey(payeeFinalPair.pk());
    Coin::RedeemScriptHash payeeScriptHash(payeeScriptPubKey);

    uint32_t lockTime = 100;
    uint64_t channelValue = 180;

    joystream::paymentchannel::Payor p(1, 0, channelValue, 1000, lockTime,contractOutPoint, payorContractPair, payorScriptHash, payeeContractPair.pk(), payeeScriptHash);

    joystream::paymentchannel::Refund R(p.refund());

    QCOMPARE(R.lockedUntil(0), lockTime);

    // The output is locked until the block height is greater than the locktime
    QCOMPARE(R.isLocked(99,1), true); // locked
    QCOMPARE(R.isLocked(100,1), true); // locked
    QCOMPARE(R.isLocked(101,1), false); // unlocked

    QCOMPARE(R.getUnspentOutput().value(), channelValue);

    QVERIFY(R.getUnspentOutput().outPoint() == contractOutPoint);
}

void Test::settlement() {

    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();
    uint32_t lockTime = 100;

    Coin::typesafeOutPoint contractOutPoint;
    joystream::paymentchannel::Commitment commitment(180, payorContractPair.pk(), payeeContractPair.pk(), lockTime);

    Coin::P2PKScriptPubKey payorScriptPubKey(payorFinalPair.pk());
    Coin::RedeemScriptHash payorScriptHash(payorScriptPubKey);

    Coin::P2PKScriptPubKey payeeScriptPubKey(payeeFinalPair.pk());
    Coin::RedeemScriptHash payeeScriptHash(payeeScriptPubKey);

    Coin::Payment toPayor(90, payorScriptHash);
    Coin::Payment toPayee(90, payeeScriptHash);

    joystream::paymentchannel::Settlement s(contractOutPoint,
                                            commitment,
                                            toPayor,
                                            toPayee);

    // Generate payee refund signature, hence using payee private key
    Coin::TransactionSignature payeePaySig = s.transactionSignature(payeeContractPair.sk());
    Coin::TransactionSignature payorPaySig = s.transactionSignature(payorContractPair.sk());

    bool validPayeeSettlementSig = s.validatePayeeSignature(payeePaySig.sig());
    QVERIFY(validPayeeSettlementSig);

    bool validPayorSettlementSig = s.validatePayorSignature(payorPaySig.sig());
    QVERIFY(validPayorSettlementSig);

    QVERIFY(s.fee() == 0);

}

void Test::channel() {

}

void Test::paychan_one_to_one() {

    // replace all "generate" with fixed private keys from bitcore playground

    // Setup keys
    Coin::KeyPair payorContractKeyPair = Coin::KeyPair::generate();
    Coin::RedeemScriptHash payorFinalKeyHash;

    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::RedeemScriptHash payeeFinalKeyHash;

    // Funding & allocation
    uint64_t source_amount = 3000000,
            change_amount = 200,
            contract_fee_amount = 50,
            amount_in_channel = source_amount - (change_amount + contract_fee_amount);

    // Construct contract
    auto funding = std::make_shared<Coin::UnspentP2PKHOutput>(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source_amount);
    Contract c(Coin::UnspentOutputSet({funding}));

    // Terms
    uint32_t lockTime = 1000;
    uint64_t price = 8;

    c.addCommitment(Commitment(amount_in_channel, payorContractKeyPair.pk(), payeeContractKeyPair.pk(), lockTime));

    Coin::Payment change(change_amount, Coin::RedeemScriptHash());
    c.setChange(change);

    // Derive anchor
    Coin::Transaction contractTx = c.transaction();
    Coin::typesafeOutPoint anchor(Coin::TransactionId::fromTx(contractTx), 0);

    // Setup payor
    joystream::paymentchannel::Payor payor(price,
                                          0,
                                          amount_in_channel,
                                          0,
                                          lockTime,
                                          anchor,
                                          payorContractKeyPair,
                                          payorFinalKeyHash,
                                          payeeContractKeyPair.pk(),
                                          payeeFinalKeyHash);

    // Setup payee
    joystream::paymentchannel::Payee payee(0,
                                           lockTime,
                                           price,
                                           amount_in_channel,
                                           1,
                                           anchor,
                                           payeeContractKeyPair,
                                           payeeFinalKeyHash,
                                           payorContractKeyPair.pk(),
                                           payorFinalKeyHash,
                                           Coin::Signature());

    // Make series of payments
    int number_of_payments = 10;

    for(int i = 0; i < number_of_payments; i++) {

        // Payor makes payment i
        Coin::Signature paymentSignature = payor.makePayment();

        // Payee validates payment i
        QVERIFY(payee.registerPayment(paymentSignature));
    }

}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
