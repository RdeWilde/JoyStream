/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <common/CoinWrappers.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

class Transaction {

public:

    // Constructor from members
    Transaction(const Coin::TransactionId & transactionId, quint32 version, quint32 lockTime, QDateTime seen, const Coin::BlockId & blockId, quint64 fee);

    // Constructor from record
    // BlockHeader(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Getters and seters

private:

    // Transaction id
    Coin::TransactionId _transactionId;

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

#endif // TRANSACTION_HPP

