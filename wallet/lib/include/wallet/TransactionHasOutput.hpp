/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_TRANSACTION_HAS_OUTPUT_HPP
#define WALLET_TRANSACTION_HAS_OUTPUT_HPP

#include <wallet/Transaction.hpp>
#include <wallet/Output.hpp>

class QSqlDatabase;

namespace Wallet {
namespace TransactionHasOutput {

    struct PK {

        PK();
        PK(const Transaction::PK & transactionId, quint32 index);

        // Id of transaction
        Transaction::PK _transactionId;

        // Index of output in transaction
        quint32 _index;
    };

    struct Record {

        Record();
        Record(const PK & pk, const Output::PK & output);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;

        // Input in transaction
        Output::PK _output;
    };

    // Creates table, return true IFF it worked
    bool createTable(QSqlDatabase & db);

    // Insert record, returns true IFF it was inserted
    bool insert(QSqlDatabase & db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);

    std::vector<Coin::TxOut> outputsOfTransaction(QSqlDatabase & db, const Coin::TransactionId & transactionId);

}
}

#endif // WALLET_TRANSACTION_HAS_OUTPUT_HPP

