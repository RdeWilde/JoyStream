/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#ifndef JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP
#define JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP

#include <common/TransactionId.hpp>
#include <common/KeyPair.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class TransactionId;
    class typesafeOutPoint;
}

namespace joystream {
namespace paymentchannel {

    class Commitment;
    class Refund;
    class Settlement;

    // Represents a single channel
    class Channel {

    public:

        Channel();

        Channel(const Coin::TransactionId & contractTxId,
                quint32 index,
                quint64 price,
                quint64 numberOfPaymentsMade,
                quint64 funds,
                quint64 refundFee,
                quint64 settlementFee,
                quint32 refundLockTime,
                const Coin::KeyPair & payorContractKeyPair,
                const Coin::KeyPair & payorFinalKeyPair,
                const Coin::PublicKey & payeeContractPk,
                const Coin::PublicKey & payeeFinalPk,
                const Coin::Signature & payorRefundSignature,
                const Coin::Signature & payeeRefundSignature);

        Coin::typesafeOutPoint contractOutPoint() const;

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

        // Checks the payee signature
        bool checkPayeeRefundSignature(const Coin::Signature & sig) const;

        // Amount of funds paid
        quint64 amountPaid() const;

        // Getters and setters
        Coin::TransactionId contractTxId() const;
        void setContractTxId(const Coin::TransactionId & contractTxId);

        quint32 index() const;
        void setIndex(quint32 index);

        quint64 price() const;
        void setPrice(quint64 price);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        quint64 funds() const;
        void setFunds(quint64 funds);

        Coin::KeyPair payorContractKeyPair() const;
        void setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair);

        Coin::KeyPair payorFinalKeyPair() const;
        void setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair);

        Coin::PublicKey payeeContractPk() const;
        void setPayeeContractPk(const Coin::PublicKey & payeeContractPk);

        Coin::PublicKey payeeFinalPk() const;
        void setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk);

        Coin::Signature payorRefundSignature() const;
        void setPayorRefundSignature(const Coin::Signature & payorRefundSignature);

        Coin::Signature payeeRefundSignature() const;
        void setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature);

        quint64 refundFee() const;
        void setRefundFee(quint64 refundFee);

        quint64 settlementFee() const;
        void setSettlementFee(quint64 settlementFee);

    private:

        // Transaction id of contract
        Coin::TransactionId _contractTxId;

        // Index in contract
        quint32 _index;

        // Size of single payment
        quint64 _price;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Refund fee
        quint64 _refundFee;

        // Settlement fee
        quint64 _settlementFee;

        // Lock time of refund, received in
        quint32 _refundLockTime;

        // Controls payour output of multisig
        Coin::KeyPair _payorContractKeyPair;

        // Controls final payment to payor
        Coin::KeyPair _payorFinalKeyPair;

        // Controls payee output of multisig, received in joinin_contract.pk
        Coin::PublicKey _payeeContractPk;

        // Controls payee payments, received in sign_refund.pk
        Coin::PublicKey _payeeFinalPk;

        // Controls refund for payor
        Coin::Signature _payorRefundSignature;

        // Controls refund for payee
        Coin::Signature _payeeRefundSignature;
    };

}
}


#endif // JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP
