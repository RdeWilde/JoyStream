/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_INBOUND_PAYMENT_HPP
#define WALLET_INBOUND_PAYMENT_HPP

//#include <common/CoinWrappers.hpp>
#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace InBoundPayment {

    struct Record {

        // Construct from members
        Record(quint64 id, quint64 receiveAddressWalletKeyIndex, const QString & note, const QDateTime & created);

        // Constructor from record
        // InBoundPayment(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Payment identifier
        quint64 _id;

        // Index of wallet key
        quint64 _receiveAddressWalletKeyIndex;

        // Payment note
        QString _note;

        // When payment was created
        QDateTime _created;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

    /**
    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase db, const Record::PK & pk, Record & r);
    bool exists(QSqlDatabase db, const Record::PK & pk);
    */

}
}

#endif // WALLET_INBOUND_PAYMENT_HPP
