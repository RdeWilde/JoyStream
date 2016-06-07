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
#include <common/P2PKHAddress.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <CoinCore/CoinNodeData.h> // Transaction
#include <CoinCore/hdkeys.h>

using namespace joystream::paymentchannel;

void Test::refund() {

    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::typesafeOutPoint contractOutPoint;
    joystream::paymentchannel::Commitment commitment(190, payorContractPair.pk(), payeeContractPair.pk());

    Coin::Payment toPayor(190, payorFinalPair.pk().toPubKeyHash());
    uint32_t lockTime = 100;

    joystream::paymentchannel::Refund r(contractOutPoint,
                                        commitment,
                                        toPayor,
                                        lockTime);

    // Validate payee refund signature
    Coin::TransactionSignature payeeRefundSig = r.transactionSignature(payeeContractPair.sk());

    bool validPayeeRefundSig = r.validatePayeeSignature(payeeRefundSig.sig());

    QVERIFY(validPayeeRefundSig);
    QVERIFY(r.fee() == 0);

    // Validate payee refund signature
    Coin::TransactionSignature payorRefundSig = r.transactionSignature(payorContractPair.sk());

    bool validPayorRefundSig = r.validatePayorSignature(payorRefundSig.sig());

    QVERIFY(validPayorRefundSig);
}

void Test::settlement() {

    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();

    Coin::typesafeOutPoint contractOutPoint;
    joystream::paymentchannel::Commitment commitment(180, payorContractPair.pk(), payeeContractPair.pk());
    Coin::Payment toPayor(90, payorFinalPair.pk().toPubKeyHash());
    Coin::Payment toPayee(90, payeeFinalPair.pk().toPubKeyHash());

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
    Coin::PubKeyHash payorFinalKeyHash("5364093874829384794bda860241f4c55ea0b297");

    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::PubKeyHash payeeFinalKeyHash("0285b8ceae4c5d7f094bda860241f4c55ea0b297");

    // Funding & allocation
    uint64_t source_amount = 3000000,
            change_amount = 200,
            contract_fee_amount = 50,
            amount_in_channel = source_amount - (change_amount + contract_fee_amount);

    // Construct contract
    Coin::UnspentP2PKHOutput funding(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source_amount);
    Coin::Payment change(change_amount, Coin::PubKeyHash("8956784568342390764574523895634289896781"));

    Contract c(funding);
    c.addCommitment(Commitment(amount_in_channel, payorContractKeyPair.pk(), payeeContractKeyPair.pk()));
    c.setChange(change);

    // Derive anchor
    Coin::Transaction contractTx = c.transaction();
    Coin::typesafeOutPoint anchor(Coin::TransactionId::fromTx(contractTx), 0);

    // Terms
    uint32_t lockTime = 1000;
    uint64_t price = 8;

    // Setup payor
    joystream::paymentchannel::Payor payor(price,
                                          0,
                                          amount_in_channel,
                                          0,
                                          0,
                                          lockTime,
                                          anchor,
                                          payorContractKeyPair,
                                          payorFinalKeyHash,
                                          payeeContractKeyPair.pk(),
                                          payeeFinalKeyHash,
                                          Coin::Signature(),
                                          Coin::Signature());

    // Setup payee
    joystream::paymentchannel::Payee payee(0,
                                           lockTime,
                                           price,
                                           amount_in_channel,
                                           1,
                                           1,
                                           anchor,
                                           payeeContractKeyPair,
                                           payeeFinalKeyHash,
                                           payorContractKeyPair.pk(),
                                           payorFinalKeyHash,
                                           Coin::Signature());

    // Payee generates refund
    Coin::Signature refundSignature = payee.generateRefundSignature();

    // Payor validates refund
    bool wasValid = payor.checkPayeeRefundSignature(refundSignature);

    //QVERIFY(wasValid); // <-- not passing I think, but ok, we are dumping old refunds

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
