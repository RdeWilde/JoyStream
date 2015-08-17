/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTPOINT
#define WALLET_OUTPOINT

#include <common/TransactionId.hpp>

#include <QtGlobal> // quint32

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace OutPoint {

    class Record {

    public:

        // Constructor from members
        Record(const Coin::TransactionId & transactionId, quint32 outputIndex);

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

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_OUTPOINT

