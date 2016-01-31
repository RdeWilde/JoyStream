/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef EXTENSION_CHANNEL_STATUS_HPP
#define EXTENSION_CHANNEL_STATUS_HPP

#include <paymentchannel/ChannelState.hpp>

#include <QtGlobal> // temporary until we ditch qt types

namespace joystream {
namespace extension {

    class ChannelStatus {

    public:

        // Default constructor
        ChannelStatus();

        // Constructor from members
        ChannelStatus(quint32 index,
               joystream::paymentchannel::ChannelState state,
               quint64 price,
               quint64 numberOfPaymentsMade,
               quint64 funds,
               quint32 refundLockTime);

        // Getters and setters
        quint32 index() const;
        void setIndex(quint32 index);

        joystream::paymentchannel::ChannelState state() const;
        void setState(joystream::paymentchannel::ChannelState state);

        quint64 price() const;
        void setPrice(quint64 price);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        quint64 funds() const;
        void setFunds(quint64 funds);

        quint32 refundLockTime() const;
        void setRefundLockTime(quint32 refundLockTime);

    private:

        // Index in contract
        quint32 _index;

        // Slot state
        joystream::paymentchannel::ChannelState _state;

        // Size of single payment
        quint64 _price;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Lock time of refund, received in
        quint32 _refundLockTime;
    };

}
}

#endif // EXTENSION_CHANNEL_STATUS_HPP
