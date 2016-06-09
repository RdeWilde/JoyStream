/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#ifndef JOYSTREAM_PAYMENTCHANNEL_PAYOR_HPP
#define JOYSTREAM_PAYMENTCHANNEL_PAYOR_HPP

#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class TransactionId;
    class typesafeOutPoint;
    class UnspentP2SHOutput;
    class Payment;
}

namespace joystream {
namespace paymentchannel {

    class Commitment;
    class Refund;
    class Settlement;

    // Represents a single channel

    /**
     * Manages the payor side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/JoyStream/CBEP
     */
    class Payor {

    public:

        Payor();

        Payor(quint64 price,
                quint64 numberOfPaymentsMade,
                quint64 funds,
                quint64 settlementFee,
                quint32 refundLockTime,
                const Coin::typesafeOutPoint & anchor,
                const Coin::KeyPair & payorContractKeyPair,
                const Coin::RedeemScriptHash & payorFinalScriptHash,
                const Coin::PublicKey & payeeContractPk,
                const Coin::RedeemScriptHash & payeeFinalScriptHash,
                const Coin::Signature & payorRefundSignature,
                const Coin::Signature & payeeRefundSignature);

        // Commitment for channel
        Commitment commitment() const;

        // Refund for channel
        Refund refund() const;

        // Payment for channel
        Settlement settlement() const;

        // Generates a refund signature for payor
        Coin::Signature generatePayorRefundSignature() const;

        // Generates settlement signature for payor
        Coin::Signature generatePayorSettlementSignature() const;

        // Increments payment counter, and generates/returns
        // settlement signature for payor
        Coin::Signature makePayment();

        // Checks the payee signature
        bool checkPayeeRefundSignature(const Coin::Signature &) const;

        // Amount of funds paid
        quint64 amountPaid() const;

        // Getters and setters
        quint64 price() const;
        void setPrice(quint64 price);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64);

        quint64 funds() const;
        void setFunds(quint64 funds);

        quint64 settlementFee() const;
        void setSettlementFee(quint64);

        quint32 refundLockTime() const;
        void setRefundLockTime(quint32);

        Coin::typesafeOutPoint anchor() const;
        void setAnchor(const Coin::typesafeOutPoint &);

        Coin::KeyPair payorContractKeyPair() const;
        void setPayorContractKeyPair(const Coin::KeyPair &);

        Coin::RedeemScriptHash payorFinalScriptHash() const;
        void setPayorFinalScriptHash(const Coin::RedeemScriptHash &);

        Coin::PublicKey payeeContractPk() const;
        void setPayeeContractPk(const Coin::PublicKey &);

        Coin::RedeemScriptHash payeeFinalScriptHash() const;
        void setPayeeFinalScriptHash(const Coin::RedeemScriptHash &);

    private:

        // Size of single payment
        quint64 _price;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Settlement fee
        quint64 _settlementFee;

        // Lock time of refund, received in
        quint32 _refundLockTime;

        // Anchor for channel in contract transaction
        Coin::typesafeOutPoint _anchor;

        // Controls payour output of multisig
        Coin::KeyPair _payorContractKeyPair;

        // Controls final payment to payor
        Coin::RedeemScriptHash _payorFinalScriptHash;

        // Controls payee output of multisig, received in joinin_contract.pk
        Coin::PublicKey _payeeContractPk;

        // Controls payee payments, received in sign_refund.pk
        Coin::RedeemScriptHash _payeeFinalScriptHash;
    };

}
}

#endif // JOYSTREAM_PAYMENTCHANNEL_PAYOR_HPP
