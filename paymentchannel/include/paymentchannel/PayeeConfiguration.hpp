/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef PAYMENT_CHANNEL_PAYEE_CONFIGURATION_HPP
#define PAYMENT_CHANNEL_PAYEE_CONFIGURATION_HPP

#include <paymentchannel/PayeeState.hpp>

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace paymentchannel {

    class PayeeConfiguration {

    public:

        // Default constructor
        PayeeConfiguration();

        // Constructor from members
        PayeeConfiguration(PayeeState state,
                      quint64 numberOfPaymentsMade,
                      const Coin::Signature & lastValidPayorPaymentSignature,
                      quint32 lockTime,
                      quint64 price,
                      quint32 maximumNumberOfSellers,
                      const Coin::KeyPair & payeeContractKeys,
                      const Coin::KeyPair & payeePaymentKeys,
                      const Coin::typesafeOutPoint & contractOutPoint,
                      const Coin::PublicKey & payorContractPk,
                      const Coin::PublicKey & payorFinalPk,
                      quint64 funds);

        // Getters and setters
        PayeeState state() const;
        void setState(PayeeState state);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        Coin::Signature lastValidPayorPaymentSignature() const;
        void setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature);

        quint32 lockTime() const;
        void setLockTime(quint32 lockTime);

        quint64 price() const;
        void setPrice(quint64 price);

        quint32 maximumNumberOfSellers() const;
        void setMaximumNumberOfSellers(quint32 maximumNumberOfSellers);

        Coin::KeyPair payeeContractKeys() const;
        void setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys);

        Coin::KeyPair payeePaymentKeys() const;
        void setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint);

        Coin::PublicKey payorContractPk() const;
        void setPayorContractPk(const Coin::PublicKey & payorContractPk);

        Coin::PublicKey payorFinalPk() const;
        void setPayorFinalPk(const Coin::PublicKey & payorFinalPk);

        quint64 funds() const;
        void setFunds(quint64 funds);

    private:

        // Payee state
        PayeeState _state;

        // The number of payments which have been successfully made
        quint64 _numberOfPaymentsMade;

        // The last valid payment signature received, corresponds to _numberOfPaymentsMade
        Coin::Signature _lastValidPayorPaymentSignature;

        // Payment channel lock time
        quint32 _lockTime;

        // Price increment per payment
        quint64 _price;

        // Maximum number of non change outputs of payment channel contract
        quint32 _maximumNumberOfSellers;

        // Controls payee portion of contract output
        Coin::KeyPair _payeeContractKeys;

        // Controls payee output in payment _lastValidPaymentSignature
        Coin::KeyPair _payeePaymentKeys;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Payor key in contract output
        Coin::PublicKey _payorContractPk;

        // Payor key in output in refund and payment
        Coin::PublicKey _payorFinalPk;

        // Amount (#satoshies) assigned to contract output
        quint64 _funds;

    };
}
}

#endif // PAYMENT_CHANNEL_PAYEE_CONFIGURATION_HPP

