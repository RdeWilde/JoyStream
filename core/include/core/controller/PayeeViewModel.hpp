/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYEE_VIEW_MODEL_HPP
#define PAYEE_VIEW_MODEL_HPP

#include <core/extension/PaymentChannel/Payee.hpp>

#include <QObject>

class PayeeViewModel : public QObject
{

    Q_OBJECT

public:

    // Constructor
    PayeeViewModel(QObject * parent,
                   const Payee::Status & status);

    // Update
    void update(const Payee::Status & status);

    // Getters
    Payee::Status status() const;

signals:

    // Update
    void stateChanged(Payee::State state);
    void contractOutPointChanged(const Coin::OutPoint & o);
    void numberOfPaymentsMadeChanged(quint64 numberOfPaymentsMade);
    void lockTimeChanged(quint32 lockTime);
    void priceChanged(quint64 price);
    void fundsChanged(quint64 funds);
    void balanceChanged(quint64 balance);

private:

    // Payee status
    Payee::Status _status;
};

#endif // PAYEE_VIEW_MODEL_HPP
