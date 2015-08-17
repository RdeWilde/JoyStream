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

    class Record {

    public:

        // Construct from members
        Record(const Coin::TransactionId & transactionId, quint32 index, const Output::Record & output);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        Coin::TransactionId transactionId() const;
        void setTransactionId(const Coin::TransactionId & transactionId);

        quint32 index() const;
        void setIndex(quint32 index);

        Output::Record output() const;
        void setOutput(const Output::Record & output);

    private:

        // Id of transaction
        Coin::TransactionId _transactionId;

        // Index of output in transaction
        quint32 _index;

        // Input in transaction
        Output::Record _output;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // TRANSACTION_HAS_OUTPUT_HPP

