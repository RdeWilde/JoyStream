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
    Commitment commitment(190, payorContractPair.pk(), payeeContractPair.pk());

    Coin::Payment toPayor(190, payorFinalPair.pk().toPubKeyHash());
    uint32_t lockTime = 100;

    Refund r(contractOutPoint,
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
    Commitment commitment(180, payorContractPair.pk(), payeeContractPair.pk());
    Coin::Payment toPayor(90, payorFinalPair.pk().toPubKeyHash());
    Coin::Payment toPayee(90, payeeFinalPair.pk().toPubKeyHash());

    Settlement s(contractOutPoint,
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

    // Use test seed to generate hd key chain
    //Coin::HDKeychain chain = WALLET_SEED.generateHDKeychain();
    //chain.

    // Setup keys
    Coin::KeyPair payorContractKeyPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalKeyPair = Coin::KeyPair::generate();

    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalKeyPair = Coin::KeyPair::generate();

    // Setup channels
    std::vector<Payor::Channel::Configuration> channels;

    uint64_t source = 3000000,
            change = 200,
            contract_fee = 50,
            funds_in_channel = source - (change + contract_fee);

    uint64_t price = 8;

    uint32_t lockTime = 1000;

    channels.push_back(Payor::Channel::Configuration(0,
                                                     Payor::Channel::State::unassigned,
                                                     price,
                                                     0,
                                                     funds_in_channel, // total funds
                                                     payorContractKeyPair,
                                                     payorFinalKeyPair,
                                                     payeeContractKeyPair.pk(),
                                                     payeeFinalKeyPair.pk(),
                                                     Coin::Signature(),
                                                     Coin::Signature(),
                                                     0,
                                                     0,
                                                     lockTime));
    // Setup payor
    Payor payor(Payor::Configuration(NETWORK_TYPE,
                                     Payor::State::waiting_for_full_set_of_sellers,
                                     channels,
                                     Coin::UnspentP2PKHOutput(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source),
                                     Coin::KeyPair::generate(),
                                     change, // change value
                                     contract_fee, // contract fee
                                     Coin::TransactionId(),
                                     0));

    payor.assignUnassignedSlot(price, payeeContractKeyPair.pk(), payeeFinalKeyPair.pk(), lockTime);
    Coin::TransactionId contractId = Coin::TransactionId::fromTx(payor.contractTransaction());

    // Setup payee
    Payee payee(Payee::Configuration(Payee::State::waiting_for_payor_information,
                                     0,
                                     Coin::Signature(),
                                     lockTime,
                                     price,
                                     1,
                                     payeeContractKeyPair,
                                     payeeFinalKeyPair,
                                     Coin::typesafeOutPoint(),
                                     payorContractKeyPair.pk(),
                                     payorFinalKeyPair.pk(),
                                     funds_in_channel));


    payee.registerPayorInformation(Coin::typesafeOutPoint(contractId, 0),
                                   payorContractKeyPair.pk(),
                                   payorFinalKeyPair.pk(),
                                   funds_in_channel);

    // Payee generates refund
    Coin::Signature refundSignature = payee.generateRefundSignature();

    // Payor validates refund

    bool wasValid = payor.processRefundSignature(0, refundSignature);

    QVERIFY(wasValid);

    // Make series of payments
    int number_of_payments = 10;

    for(int i = 0; i < number_of_payments; i++) {

        // Payor makes payment i
        Q_ASSERT(payor.incrementPaymentCounter(0) == i+1);
        Coin::Signature paymentSignature = payor.getPresentPaymentSignature(0);

        // Payee validates payment i
        QVERIFY(payee.registerPayment(paymentSignature));
    }

}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
