/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef EXTENSION_PAYEE_STATUS_HPP
#define EXTENSION_PAYEE_STATUS_HPP

#include <paymentchannel/PayeeState.hpp>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace extension {

    class PayeeStatus {

    public:

        // Default constructor
        PayeeStatus();

        // Constructor from members
        PayeeStatus(joystream::paymentchannel::PayeeState state, quint64 numberOfPaymentsMade, quint32 lockTime, quint64 price, const Coin::typesafeOutPoint & contractOutPoint, quint64 funds);

        // Getters and setters
        joystream::paymentchannel::PayeeState state() const;
        void setState(joystream::paymentchannel::PayeeState state);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        quint32 lockTime() const;
        void setLockTime(quint32 lockTime);

        quint64 price() const;
        void setPrice(quint64 price);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint);

        quint64 funds() const;
        void setFunds(quint64 funds);

    private:

        // Payee state
        joystream::paymentchannel::PayeeState _state;

        // The number of payments which have been successfully made
        quint64 _numberOfPaymentsMade;

        // Payment channel lock time
        quint32 _lockTime;

        // Price increment per payment
        quint64 _price;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Amount (#satoshies) assigned to contract output
        quint64 _funds;
    };

}
}

#endif

