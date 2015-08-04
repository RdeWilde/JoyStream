/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef SLOT_HPP
#define SLOT_HPP

#include <wallet/CoinWrappers.hpp>

class QSqlQuery;

class Slot
{

public:

    /**
     * @brief States of channel.
     */
    enum class State {
        unassigned,
        assigned,
        refund_signed
    };

    static quint8 encodeState(State state);
    static State decodeState(quint8 state);

    Slot();

private:

    quint64 _payerId;

    // Index in contract
    quint32 _index;


    state
    price
    numberOfPaymentsMade
    funds
    refundLockTime
    payerContractWalletKeyId
    payerFinalWalletKeyId
    payeeContractPublicKey
    payeeFinalPublicKey
    refundSignature
    lastPaymentSignature
    refundFee
    paymentFee
    refundTransactionId



    // Slot state
    State _state;

    // Size of single payment
    quint64 _price;

    // Number of payments made
    quint64 _numberOfPaymentsMade;

    // Funds allocated to output
    quint64 _funds;

    // Controls payour output of multisig
    KeyPair _payorContractKeyPair;

    // Controls final payment to payor
    KeyPair _payorFinalKeyPair;

    // Controls payee output of multisig, received in joinin_contract.pk
    PublicKey _payeeContractPk;

    // Controls payee payments, received in sign_refund.pk
    PublicKey _payeeFinalPk;

    // Controls refund for payor
    Signature _payorRefundSignature;

    // Controls refund for payee
    Signature _payeeRefundSignature;

    // Fee used in refund transaction, is unlikely to vary across slots,
    quint64 _refundFee;

    // Fee used in payment transaction
    quint64 _paymentFee;

    // Lock time of refund, received in
    quint32 _refundLockTime;
};

#endif // SLOT_HPP
