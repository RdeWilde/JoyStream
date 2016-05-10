/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_VIEW_MODEL_PAYEE_HPP
#define JOYSTREAM_CORE_VIEW_MODEL_PAYEE_HPP

#include <extension/PayeeStatus.hpp>

#include <QObject>

namespace joystream {
namespace core {
namespace viewmodel {

    class Payee : public QObject {

        Q_OBJECT

    public:

        // Constructor
        Payee(QObject * parent, const joystream::extension::PayeeStatus & status);

        // Update
        void update(const joystream::extension::PayeeStatus & status);

        // Getters
        joystream::extension::PayeeStatus status() const;

    signals:

        // Update
        void stateChanged(joystream::paymentchannel::PayeeState state);
        void contractOutPointChanged(const Coin::typesafeOutPoint & o);
        void numberOfPaymentsMadeChanged(quint64 numberOfPaymentsMade);
        void lockTimeChanged(quint32 lockTime);
        void priceChanged(quint64 price);
        void fundsChanged(quint64 funds);
        void balanceChanged(quint64 balance);

    private:

        // Payee status
        joystream::extension::PayeeStatus _status;
    };

}
}
}

#endif // JOYSTREAM_CORE_VIEW_MODEL_PAYEE_HPP
