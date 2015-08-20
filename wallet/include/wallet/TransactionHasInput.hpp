/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_TRANSACTION_HAS_INPUT_HPP
#define WALLET_TRANSACTION_HAS_INPUT_HPP

#include <wallet/Transaction.hpp>
#include <wallet/Input.hpp>

class QSqlDatabase;
class QSqlRecord;

namespace Wallet {
namespace TransactionHasInput {

    struct PK {

        PK();
        PK(const Transaction::PK & transactionId, quint32 index);

        // Id of transaction
        Transaction::PK _transactionId;

        // Index of input in transaction
        quint32 _index;

    };

    struct Record {

        Record();
        Record(const PK & pk, const Input::PK & input);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;

        // Input in transaction
        Input::PK _input;
    };

    // Query which creates table corresponding to entity
    bool createTable(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    bool insert(QSqlDatabase & db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);

}
}

#endif // WALLET_TRANSACTION_HAS_INPUT_HPP
