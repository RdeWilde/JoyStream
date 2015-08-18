/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef TRANSACTION_HAS_OUTPUT_HPP
#define TRANSACTION_HAS_OUTPUT_HPP

#include <wallet/Transaction.hpp>
#include <wallet/Output.hpp>

class QSqlDatabase;

namespace Wallet {
namespace TransactionHasOutput {

    struct Record {

        struct PK {

            PK();
            PK(const Coin::TransactionId & transactionId, quint32 index);

            // Id of transaction
            Coin::TransactionId _transactionId;

            // Index of output in transaction
            quint32 _index;
        };

        Record();
        Record(const PK & pk, const Output::PK & output);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Primary key
        PK _pk;

        // Input in transaction
        Output::PK _output;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTable(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const Record::PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const Record::PK & pk);

}
}

#endif // TRANSACTION_HAS_OUTPUT_HPP

