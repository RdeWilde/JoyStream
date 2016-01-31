/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef PAYMENT_CHANNEL_CHANNEL_CONFIGURATION_HPP
#define PAYMENT_CHANNEL_CHANNEL_CONFIGURATION_HPP

#include <paymentchannel/ChannelState.hpp>
#include <common/KeyPair.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace paymentchannel {

    class ChannelConfiguration {

    public:

        // Default constructor.
        ChannelConfiguration();

        // Constructor from members.
        ChannelConfiguration(quint32 index,
                              ChannelState state,
                              quint64 price,
                              quint64 numberOfPaymentsMade,
                              quint64 funds,
                              const Coin::KeyPair & payorContractKeyPair,
                              const Coin::KeyPair & payorFinalKeyPair,
                              const Coin::PublicKey & payeeContractPk,
                              const Coin::PublicKey & payeeFinalPk,
                              const Coin::Signature & payorRefundSignature,
                              const Coin::Signature & payeeRefundSignature,
                              quint64 refundFee,
                              quint64 paymentFee,
                              quint32 refundLockTime);

        // Getters and setters
        quint32 index() const;
        void setIndex(quint32 index);

        ChannelState state() const;
        void setState(ChannelState state);

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

        quint64 paymentFee() const;
        void setPaymentFee(quint64 paymentFee);

        quint32 refundLockTime() const;
        void setRefundLockTime(quint32 refundLockTime);


    private:

        // Index in contract
        quint32 _index;

        // Slot state
        ChannelState _state;

        // Size of single payment
        quint64 _price;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

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

        // Fee used in refund transaction, is unlikely to vary across slots,
        quint64 _refundFee;

        // Fee used in payment transaction
        quint64 _paymentFee;

        // Lock time of refund, received in
        quint32 _refundLockTime;
    };

}
}

#endif // PAYMENT_CHANNEL_CONFIGURATION_CHANNEL

