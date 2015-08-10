/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef OUTPOINT
#define OUTPOINT

#include <common/TransactionId.hpp>
//#include <common/CoinWrappers.hpp>

#include <QtGlobal> // quint32

class QSqlQuery;
class QSqlDatabase;

class OutPoint {

public:

    // Constructor from members
    OutPoint(const Coin::TransactionId & transactionId, quint32 outputIndex);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Getters and setters
    Coin::TransactionId transactionId() const;
    void setTransactionId(const Coin::TransactionId & transactionId);

    quint32 outputIndex() const;
    void setOutputIndex(quint32 outputIndex);

private:

    // Transaction id
    Coin::TransactionId _transactionId;

    // Index of transaction output
    quint32 _outputIndex;
};

#endif // OUTPOINT

