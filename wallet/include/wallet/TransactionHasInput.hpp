/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_TRANSACTION_HAS_INPUT_HPP
#define WALLET_TRANSACTION_HAS_INPUT_HPP

//#include <common/CoinWrappers.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/Input.hpp>

class QSqlDatabase;

namespace Wallet {
namespace TransactionHasInput {

    struct Record {

        struct PK {

            PK();
            PK(const Coin::TransactionId & transactionId, quint32 index);

            // Id of transaction
            Coin::TransactionId _transactionId;

            // Index of input in transaction
            quint32 _index;

        };

        Record();
        Record(const PK & pk, const Input::Record & input);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

    private:

        // Primary key
        PK _pk;

        // Input in transaction
        Input::Record _input;
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

#endif // WALLET_TRANSACTION_HAS_INPUT_HPP
