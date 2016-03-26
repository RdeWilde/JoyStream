/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#ifndef JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP
#define JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP

#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class TransactionId;
    class typesafeOutPoint;
    class UnspentP2PKHOutput;
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
    class Channel {

    public:

        Channel();

        Channel(quint64 price,
                quint64 numberOfPaymentsMade,
                quint64 funds,
                quint64 refundFee,
                quint64 settlementFee,
                quint32 refundLockTime,
                const Coin::typesafeOutPoint & anchor,
                const Coin::KeyPair & payorContractKeyPair,
                const Coin::PubKeyHash & payorFinalPkHash,
                const Coin::PublicKey & payeeContractPk,
                const Coin::PubKeyHash & payeeFinalPkHash,
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

        quint64 refundFee() const;
        void setRefundFee(quint64);

        quint64 settlementFee() const;
        void setSettlementFee(quint64);

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

        Coin::Signature payorRefundSignature() const;
        void setPayorRefundSignature(const Coin::Signature &);

        Coin::Signature payeeRefundSignature() const;
        void setPayeeRefundSignature(const Coin::Signature &);

    private:

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

        // Controls refund for payor
        Coin::Signature _payorRefundSignature;

        // Controls refund for payee
        Coin::Signature _payeeRefundSignature;
    };

    /**
     * Manages the payor side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/JoyStream/CBEP
     */

    // Suggestive contract transaction fee
    quint64 requiredFee(int numberOfPayees, quint64 feePerKb);

    // Create and return contract transaction
    Coin::Transaction contractTransaction(const Coin::UnspentP2PKHOutput & funding,
                                          const std::vector<Commitment> & commitments,
                                          const Coin::Payment & change);

    // Create and return contract transaction, and set outputs as anchors in corresponding channels
    Coin::Transaction anchor(const Coin::UnspentP2PKHOutput & funding,
                             std::vector<Channel> & channels,
                             const Coin::Payment & change);

}
}


#endif // JOYSTREAM_PAYMENT_CHANNEL_CHANNEL_HPP
