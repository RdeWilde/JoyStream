/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTBOUND_PAYMENT_HPP
#define WALLET_OUTBOUND_PAYMENT_HPP

//#include <common/CoinWrappers.hpp>
#include <common/P2PKHAddress.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace OutBoundPayment {

    class Record {

    public:

        // Constructor from members
        Record(quint64 id, const Coin::P2PKHAddress & toAddress, quint64 changeAddressWalletKeyIndex, quint64 amount, quint64 fee, const QString & note, const QDateTime & created);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        quint64 id() const;
        void setId(quint64 id);

        Coin::P2PKHAddress toAddress() const;
        void setToAddress(const Coin::P2PKHAddress & toAddress);

        quint64 changeAddressWalletKeyIndex() const;
        void setChangeAddressWalletKeyIndex(quint64 changeAddressWalletKeyIndex);

        quint64 amount() const;
        void setAmount(quint64 amount);

        quint64 fee() const;
        void setFee(quint64 fee);

        QString note() const;
        void setNote(const QString & note);

        QDateTime created() const;
        void setCreated(const QDateTime & created);

    private:

        // Payment identifier
        quint64 _id;

        // Address to which payment is supposed to be made
        Coin::P2PKHAddress _toAddress;

        // Index of wallet key
        quint64 _changeAddressWalletKeyIndex;

        // Number of satoshies for payment (excluding payment)
        quint64 _amount;

        // Number of satoshies to use for fee for payment
        quint64 _fee;

        // Payment note
        QString _note;

        // When payment was created
        QDateTime _created;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_OUTBOUND_PAYMENT_HPP
