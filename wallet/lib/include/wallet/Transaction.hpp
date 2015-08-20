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

#include <QVariant>
#include <QDateTime>
#include <QSqlRecord>

class QSqlQuery;
class QSqlDatabase;

namespace Coin {
    class Transaction;
}

namespace Wallet {
namespace Transaction {

    struct ChainMembership {

        enum class Status {
            NOT_MINED,
            MINED_ON_FORK,
            MINED_ON_MAINCHAIN
        };

        Status _status;

        // Block to which transaction belongs.
        // Only valid if _status != NOT_MINED
        Coin::BlockId _block;

        // Height of given block
        // Only valid if _status != NOT_MINED
        quint64 _blockHeight;
    };

    typedef Coin::TransactionId PK;

    struct Record {

        Record();
        Record(const PK & pk,
               quint32 version,
               quint32 lockTime,
               const QDateTime & seen,
               const QVariant & fee);

        Record(const Coin::Transaction & transaction, const QDateTime & seen);
        Record(const QSqlRecord & record);

        Coin::Transaction toTransaction() const;

        // Transaction id
        PK _pk; //transactionId;

        // Version of transaction
        quint32 _version;

        // Transaction lock time
        quint32 _lockTime;

        // When transaction was first seen on network
        QDateTime _seen;

        // Fee associated with transaction
        // Change to QVariant, to support the possibility of NULL valuess
        // quint64 _fee;
        QVariant _fee;

        quint64 fee() const;
    };

    // Creates table
    bool createTable(QSqlDatabase & db);

    // Insert record, returns true IFF it worked
    bool insert(QSqlDatabase & db, const Record & record);

    // Lists all transactions in wallet
    QList<Record> allTransactions(QSqlDatabase & db);

    // Count number of transactions i wallet
    quint64 getTransactionCount(QSqlDatabase & db);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);
}
}

#endif // WALLET_TRANSACTION_HPP
