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
#include <common/RelativeLockTime.hpp>

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
                Coin::RelativeLockTime refundLockTime,
                const Coin::typesafeOutPoint & anchor,
                const Coin::KeyPair & payorContractKeyPair,
                const Coin::PubKeyHash & payorFinalPkHash,
                const Coin::PublicKey & payeeContractPk,
                const Coin::PubKeyHash & payeeFinalPkHash);

        // Commitment for channel
        Commitment commitment() const;

        // Refund for channel
        Refund refund() const;

        // Payment for channel
        Settlement settlement() const;

        // Generates settlement signature for payor
        Coin::Signature generatePayorSettlementSignature() const;

        // Increments payment counter, and generates/returns
        // settlement signature for payor
        Coin::Signature makePayment();

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

        Coin::RelativeLockTime refundLockTime() const;
        void setRefundLockTime(Coin::RelativeLockTime);

        Coin::typesafeOutPoint anchor() const;
        void setAnchor(const Coin::typesafeOutPoint &);

        Coin::KeyPair payorContractKeyPair() const;
        void setPayorContractKeyPair(const Coin::KeyPair &);

        Coin::PubKeyHash payorFinalPkHash() const;
        void setPayorFinalPkHash(const Coin::PubKeyHash &);

        Coin::PublicKey payeeContractPk() const;
        void setPayeeContractPk(const Coin::PublicKey &);

        Coin::PubKeyHash payeeFinalPkHash() const;
        void setPayeeFinalPkHash(const Coin::PubKeyHash &);

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
        Coin::RelativeLockTime _refundLockTime;

        // Anchor for channel in contract transaction
        Coin::typesafeOutPoint _anchor;

        // Controls payour output of multisig
        Coin::KeyPair _payorContractKeyPair;

        // Controls final payment to payor
        Coin::PubKeyHash _payorFinalPkHash;

        // Controls payee output of multisig, received in joinin_contract.pk
        Coin::PublicKey _payeeContractPk;

        // Controls payee payments, received in sign_refund.pk
        Coin::PubKeyHash _payeeFinalPkHash;
    };

}
}

#endif // JOYSTREAM_PAYMENTCHANNEL_PAYOR_HPP
