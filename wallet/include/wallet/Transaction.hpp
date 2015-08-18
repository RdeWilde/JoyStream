/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_TRANSACTION_HPP
#define WALLET_TRANSACTION_HPP

#include <common/TransactionId.hpp>
#include <common/BlockId.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Coin {
    class Transaction;
}

namespace Wallet {
namespace Transaction {

    class Record {

    public:

        typedef Coin::TransactionId PK;


        Record();
        Record(const PK & pk,
               quint32 version,
               quint32 lockTime,
               QDateTime seen,
               const Coin::BlockId & blockId,
               quint64 fee);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

    private:

        // Transaction id
        PK _pk; //transactionId;

        // Version of transaction
        quint32 _version;

        // Transaction lock time
        quint32 _lockTime;

        // When transaction was first seen on network
        QDateTime _seen;

        // Block id of block to which transaction belongs
        Coin::BlockId _blockId;

        // Fee associated with transaction
        quint64 _fee;

    };

    // Query which creates table corresponding to entity
    QSqlQuery createTable(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

    // Tries to recover the transaction with the given wallet it
    Record getTransaction(QSqlDatabase db, const Record::PK & pk);

    // Lists all transactions in wallet
    QList<Record> allTransactions(QSqlDatabase db);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const Record::PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const Record::PK & pk);
}
}

#endif // WALLET_TRANSACTION_HPP

