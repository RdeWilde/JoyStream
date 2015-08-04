/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef INBOUND_PAYMENT_HPP
#define INBOUND_PAYMENT_HPP

#include <wallet/CoinWrappers.hpp>

#include <QDateTime>

class QSqlQuery;

class InBoundPayment {

public:

    // Construct from members
    InBoundPayment(quint64 id, quint64 receiveAddressWalletKeyIndex, const QString & note, const QDateTime & created);

    // Constructor from record
    // InBoundPayment(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    quint64 paymentId() const;
    void setPaymentId(quint64 paymentId);

    quint64 receiveAddressWalletKeyIndex() const;
    void setReceiveAddressWalletKeyIndex(quint64 receiveAddressWalletKeyIndex);

    QString note() const;
    void setNote(const QString & note);

    QDateTime created() const;
    void setCreated(const QDateTime & created);

private:

    // Payment identifier
    quint64 _id;

    // Index of wallet key
    quint64 _receiveAddressWalletKeyIndex;

    // Payment note
    QString _note;

    // When payment was created
    QDateTime _created;
};

#endif // INBOUND_PAYMENT_HPP

