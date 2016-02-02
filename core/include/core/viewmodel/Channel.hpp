/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_VIEWMODEL_CHANNEL_HPP
#define JOYSTREAM_CORE_VIEWMODEL_CHANNEL_HPP

#include <extension/ChannelStatus.hpp>

#include <QObject>

namespace joystream {
namespace core {
namespace viewmodel {

    class Channel : public QObject {

        Q_OBJECT

    public:

        // Constructor
        Channel(QObject * parent,
                 quint32 index,
                 const joystream::extension::ChannelStatus & status);

        // Getters
        quint32 index() const;
        joystream::extension::ChannelStatus status() const;

    public slots:

        // Update status
        void update(const joystream::extension::ChannelStatus & status);

    signals:

        // Status change
        void stateChanged(joystream::paymentchannel::ChannelState state);
        void fundsChanged(quint64 funds);
        void refundLockTimeChanged(quint32 refundLockTime);
        void priceChanged(quint64 price);
        void numberOfPaymentsMadeChanged(quint64 numberOfPaymentsMade);
        void balanceChanged(quint64 balance);

    private:

        // Channel index in pyor
        quint32 _index;

        // Status of channel
        joystream::extension::ChannelStatus _status;
    };

}
}
}

#endif // JOYSTREAM_CORE_VIEWMODEL_CHANNEL_HPP
