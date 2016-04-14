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

    // Setup keys
    Coin::KeyPair payorContractKeyPair = Coin::KeyPair::generate();
    Coin::PubKeyHash payorFinalKeyHash("5364093874829384794bda860241f4c55ea0b297");

    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::PubKeyHash payeeFinalKeyHash("0285b8ceae4c5d7f094bda860241f4c55ea0b297");

    // Setup payor
    // *************
    std::vector<joystream::paymentchannel::Payor> channels;

    uint64_t source_amount = 3000000,
            change_amount = 200,
            contract_fee_amount = 50,
            amount_in_channel = source_amount - (change_amount + contract_fee_amount);

    uint64_t price = 8;

    uint32_t lockTime = 1000;

    channels.push_back(joystream::paymentchannel::Payor(price,
                                                          0,
                                                          amount_in_channel, // total funds
                                                          0,
                                                          0,
                                                          lockTime,
                                                          Coin::typesafeOutPoint(Coin::TransactionId(), 0), // <-- reset in anchoring
                                                          payorContractKeyPair,
                                                          payorFinalKeyHash,
                                                          payeeContractKeyPair.pk(),
                                                          payeeFinalKeyHash,
                                                          Coin::Signature(),
                                                          Coin::Signature()));

    // Anchor channels in contract
    Coin::UnspentP2PKHOutput funding(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source_amount);
    Coin::Payment change(change_amount, Coin::PubKeyHash("8956784568342390764574523895634289896781"));
    Coin::Transaction contractTx = joystream::paymentchannel::anchor(funding, channels, change);

    // Get channel
    joystream::paymentchannel::Payor & channel = channels[0];

    // Setup payee
    // *************
    joystream::paymentchannel::Payee payee(0,
                                           lockTime,
                                           price,
                                           amount_in_channel,
                                           1,
                                           1,
                                           channel.anchor(),
                                           payeeContractKeyPair,
                                           payeeFinalKeyHash,
                                           payorContractKeyPair.pk(),
                                           payorFinalKeyHash,
                                           Coin::Signature());

    // Payee generates refund
    Coin::Signature refundSignature = payee.generateRefundSignature();

    // Payor validates refund
    bool wasValid = channel.checkPayeeRefundSignature(refundSignature);

    //QVERIFY(wasValid);

    // Make series of payments
    int number_of_payments = 10;

    for(int i = 0; i < number_of_payments; i++) {

        // Payor makes payment i
        channel.setNumberOfPaymentsMade(channel.numberOfPaymentsMade() + 1);
        Coin::Signature paymentSignature = channel.generatePayorSettlementSignature();

        // Payee validates payment i
        QVERIFY(payee.registerPayment(paymentSignature));
    }

}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
