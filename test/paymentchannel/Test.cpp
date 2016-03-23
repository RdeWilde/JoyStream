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
#include <CoinCore/hdkeys.h>

#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Payee.hpp>

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
    std::vector<joystream::paymentchannel::Channel> channels;

    uint64_t source = 3000000,
            change = 200,
            contract_fee = 50,
            funds_in_channel = source - (change + contract_fee);

    uint64_t price = 8;

    uint32_t lockTime = 1000;

    channels.push_back(joystream::paymentchannel::Channel(price,
                                                          0,
                                                          funds_in_channel, // total funds
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

    joystream::paymentchannel::Payor payor(channels,
                                           Coin::UnspentP2PKHOutput(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source),
                                           Coin::P2PKHAddress(),
                                           change, // change value
                                           contract_fee, // contract fee
                                           Coin::Transaction()); // <-- reset in anchoring
    payor.anchor();

    joystream::paymentchannel::Channel & channel = payor.channel(0);

    // Setup payee
    // *************
    joystream::paymentchannel::Payee payee(0,
                                           lockTime,
                                           price,
                                           funds_in_channel,
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

    QVERIFY(wasValid);

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
