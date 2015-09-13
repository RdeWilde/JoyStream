/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <Test.hpp>

#include <common/Seed.hpp>
#include <CoinCore/hdkeys.h>

#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Payee.hpp>

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
                                                     Payor::Channel::State::assigned,
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
                                     Payor::State::waiting_for_full_set_of_refund_signatures,
                                     channels,
                                     Coin::UnspentP2PKHOutput(Coin::KeyPair(), Coin::typesafeOutPoint(), source),
                                     Coin::KeyPair::generate(),
                                     change, // change value
                                     contract_fee, // contract fee
                                     Coin::TransactionId(),
                                     0));

    // Setup payee
    Payee payee(Payee::Configuration(Payee::State::has_all_information_required,
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

    // Payee generates refund
    Coin::Signature refundSignature = payee.generateRefundSignature();

    // Payor validates refund
    bool wasValid = payor.processRefundSignature(0, refundSignature);

    QVERIFY(wasValid);

    // Make series of payments
    int number_of_paymnts = 10;

    for(int i = 0; i < number_of_paymnts; i++) {

        // Payor makes payment i
        Q_ASSERT(payor.incrementPaymentCounter(0) == i+1);
        Coin::Signature paymentSignature = payor.getPresentPaymentSignature(0);

        // Payee validates payment i
        QVERIFY(payee.registerPayment(paymentSignature));
    }

}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
